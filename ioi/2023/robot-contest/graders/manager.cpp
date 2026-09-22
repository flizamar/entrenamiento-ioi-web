#include <cassert>
#include <cinttypes>
#include <csignal>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include "testlib.h"

const int MAX_COLOR = 19;
const int MAX_STATE = 5153632; // = (MAX_COLOR + 3) ^ 5
const int MAX_STEPS = 500000;

const std::string output_secret = "p71c1rw3EvfjFcMVWG0EOedGbWDX5uUp";

// Each instruction is encoded as a byte.
// The value zero indicates an unset instruction.
// Otherwise, the lowest 5 bits are the color, the highest 3 are the action.
// See the functions 'action' and 'color'.

using instruction = uint8_t;
instruction program[MAX_STATE];

char action(instruction instr)
{
    return "_HWSENT_"[instr >> 5];
}

int color(instruction instr)
{
    return instr & 0x1F;
}

void validate_instr(instruction instr)
{
    if (instr && (color(instr) > MAX_COLOR || action(instr) == '_'))
    {
        quit(_pv, "Invalid instruction");
    }
}

int encode_state(const std::vector<int> &S)
{
    int state = 0;
    for (int s : S)
    {
        state = (MAX_COLOR + 3) * state + s + 2;
    }
    return state;
}

std::string print_state(const std::vector<int> &S)
{
    std::string s = "[" + std::to_string(S[0]);
    for (int i = 1; i <= 4; i++)
    {
        s += ", ";
        s += std::to_string(S[i]);
    }
    s += "]";
    return s;
}

int H, W;

struct sim_state
{
    std::vector<std::vector<int>> cells;
    int rx = 1, ry = 1;

    bool step()
    {
        std::vector<int> state = {
            cells[rx][ry],
            cells[rx][ry - 1],
            cells[rx + 1][ry],
            cells[rx][ry + 1],
            cells[rx - 1][ry]};
        uint8_t instr = program[encode_state(state)];
        if (!instr)
        {
            quit(_wa, "Unexpected state: " + print_state(state));
        }
        cells[rx][ry] = color(instr);
        switch (action(instr))
        {
        case 'T':
            return true;
        case 'W':
            ry--;
            break;
        case 'S':
            rx++;
            break;
        case 'E':
            ry++;
            break;
        case 'N':
            rx--;
            break;
        case 'H':
            break;
        default:
            quit(_fail, "Unknown action");
        }
        if (cells[rx][ry] < 0)
        {
            quit(_wa, "Invalid move: S = " + print_state(state) +
                          ", A = '" + action(instr) + "'");
        }
        return false;
    }
};

const std::vector<std::pair<int, int>> moves = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

// This is not necessary, but it gives better error messages
static void check_path(const std::vector<std::vector<int>> &cells)
{
    int x = 1, y = 1;
    int px = 0, py = 0;
    int nx, ny;
    int count = 0;
    while (true)
    {
        count++;
        bool found = false;
        for (auto m : moves)
        {
            int x2 = x + m.first;
            int y2 = y + m.second;
            if (cells[x2][y2] != 1 || (x2 == px && y2 == py))
            {
                continue;
            }
            if (found)
            {
                quit(_wa, "Not a path");
            }
            nx = x2, ny = y2;
            found = true;
        }
        if (found == (x == H && y == W))
        {
            quit(_wa, "Not a path");
        }
        if (x == H && y == W)
        {
            break;
        }
        px = x, py = y, x = nx, y = ny;
    }
    for (int i = 1; i <= H; i++)
    {
        for (int j = 1; j <= W; j++)
        {
            if (cells[i][j] == 1)
            {
                count--;
            }
        }
    }
    if (count < 0)
    {
        quit(_wa, "Cell with color 1 outside the path");
    }
}

static const char NOT_SHORTEST[] = "Not a shortest path";

static bool check_shortest(const std::vector<std::vector<int>> &cells)
{
    std::vector<std::pair<int, int>> level = {{1, 1}};
    int px = 0, py = 1;
    std::vector<std::vector<bool>> visited(H + 2, std::vector<bool>(W + 2));
    visited[1][1] = true;
    bool cleanup = true;
    bool done = false;
    while (level.size())
    {
        bool has_path = false;
        std::vector<std::pair<int, int>> next_level;
        for (auto pos : level)
        {
            int x = pos.first, y = pos.second;
            if (cells[x][y] == 1)
            {
                if (has_path || done || abs(x - px) + abs(y - py) != 1)
                {
                    quit(_wa, NOT_SHORTEST);
                }
                px = x, py = y;
                has_path = true;
                if (x == H && y == W)
                {
                    done = true;
                }
            }
            else if (x == H && y == W)
            {
                quit(_wa, NOT_SHORTEST);
            }
            else if (cells[x][y] > 1)
            {
                cleanup = false;
            }
            for (auto m : moves)
            {
                int x2 = x + m.first;
                int y2 = y + m.second;
                if (cells[x2][y2] < 0 || visited[x2][y2])
                {
                    continue;
                }
                visited[x2][y2] = true;
                next_level.emplace_back(x2, y2);
            }
        }
        if (!done && !has_path)
        {
            quit(_wa, NOT_SHORTEST);
        }
        level = std::move(next_level);
    }
    return cleanup;
}

// The list of possible error messages sent by the grader
std::set<std::string> possible_errors = {
    "Invalid array",
    "Invalid color",
    "Invalid action",
    "Same state array",
};

[[noreturn]] void pv()
{
    quit(_pv, "Possible tampering with the output");
}

void communicate(const char *output_file)
{
    std::ifstream grader_output(output_file, std::ios::binary);
    if (!grader_output)
    {
        quit(_fail, "Cannot open file");
    };
    std::string grader_secret;
    std::getline(grader_output, grader_secret);
    if (grader_secret != output_secret)
    {
        pv();
    }
    std::string grader_result;
    std::getline(grader_output, grader_result);
    if (grader_result == "PV")
    {
        std::string grader_message;
        std::getline(grader_output, grader_message);
        if (!grader_output || !possible_errors.count(grader_message))
        {
            pv();
        }
        quit(_pv, grader_message);
    }
    else if (grader_result == "OK")
    {
        grader_output.read(reinterpret_cast<char *>(program), sizeof(program));
        grader_output.peek();
        if (!grader_output || !grader_output.eof())
        {
            pv();
        }
    }
    else
    {
        pv();
    }
}

void eval_testcase(const std::vector<std::vector<int>> &cells, bool &cleanup, int &max_steps)
{
    sim_state sim{cells};
    bool done = false;
    int steps = 0;
    while (!done && ++steps <= MAX_STEPS)
    {
        done = sim.step();
    }
    if (!done)
    {
        quit(_wa, "Too many steps");
    }
    check_path(sim.cells);
    cleanup &= check_shortest(sim.cells);
    max_steps = std::max(max_steps, steps);
}

template <class F>
std::vector<std::vector<int>> transform_testcase(const std::vector<std::vector<int>> &cells, F f)
{
    std::vector<std::vector<int>> transformed(H + 2, std::vector<int>(W + 2));
    for (int i = 0; i < H + 2; i++)
    {
        for (int j = 0; j < W + 2; j++)
        {
            std::pair<int, int> pos = f(i, j);
            transformed[i][j] = cells[pos.first][pos.second];
        }
    }
    return transformed;
}

double get_score(int colors, bool cleanup, bool partial)
{
    if (!partial)
    {
        return cleanup ? 1. : 0.5;
    }
    int res;
    if (colors >= 11)
    {
        res = 12 + (19 - colors) + 8 * cleanup;
    }
    else
    {
        int score[][2] = {
            {36, 46},
            {32, 42},
            {29, 38},
            {26, 34},
            {23, 31},
        };
        res = score[std::max(colors - 6, 0)][cleanup];
    }
    return res / 46.;
}

int main(int argc, char *argv[])
{
    // Keep alive on broken pipes
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);

    testlibMode = _checker;
    ouf.init(stdout, _output);
    if (argc < 3)
    {
        quit(_fail, "Insufficient number of args for manager of 'robot'");
    }
    if (!(std::cin >> H >> W))
    {
        quit(_fail, "Invalid test data");
    }
    std::vector<std::vector<int>> cells(H + 2, std::vector<int>(W + 2));
    for (int j = 0; j <= W + 1; j++)
    {
        cells[0][j] = cells[H + 1][j] = -2;
    }
    for (int i = 1; i <= H; i++)
    {
        cells[i][0] = cells[i][W + 1] = -2;
        for (int j = 1; j <= W; j++)
        {
            bool cell;
            if (!(std::cin >> cell))
            {
                quit(_fail, "Invalid test data");
            }
            cells[i][j] = cell ? -1 : 0;
        }
    }
    std::string extra;
    std::cin >> extra;

    communicate(argv[1]);
    int used_colors = 0;
    for (instruction instr : program)
    {
        validate_instr(instr);
        if (instr)
        {
            used_colors = std::max(used_colors, color(instr));
        }
    }

    auto rotate_180 = [](int i, int j)
    { return std::make_pair(H + 1 - i, W + 1 - j); };
    auto reflect_diag1 = [](int i, int j)
    { return std::make_pair(j, i); };
    auto reflect_diag2 = [](int i, int j)
    { return std::make_pair(H + 1 - j, H + 1 - i); };

    bool cleanup = true;
    int max_steps = 0;
    eval_testcase(cells, cleanup, max_steps);
    if (extra.find('t') != std::string::npos)
    {
        eval_testcase(transform_testcase(cells, rotate_180), cleanup, max_steps);
        if (H == W)
        {
            eval_testcase(transform_testcase(cells, reflect_diag1), cleanup, max_steps);
            eval_testcase(transform_testcase(cells, reflect_diag2), cleanup, max_steps);
        }
    }
    bool partial = extra.find('p') != std::string::npos;
    double score = get_score(used_colors, cleanup, partial);
    if (score == 1)
    {
        quit(_ok);
    }
    else
    {
        quitp(score);
    }
}