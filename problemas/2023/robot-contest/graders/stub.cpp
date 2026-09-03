#include "robot.h"
#include <cassert>
#include <cinttypes>
#include <string>
#include <fstream>

static const int MAX_COLOR = 19;
static const int MAX_STATE = 5153632; // = (MAX_COLOR + 3) ^ 5

static const std::string output_secret = "p71c1rw3EvfjFcMVWG0EOedGbWDX5uUp";
static std::ofstream output;

[[noreturn]] static void die(const char *message)
{
    output << output_secret << "\n";
    output << "PV\n";
    output << message << "\n";
    output.close();
    exit(0);
}

static int encode_state(const std::vector<int> &S)
{
    int state = 0;
    for (int s : S)
    {
        state = (MAX_COLOR + 3) * state + s + 2;
    }
    return state;
}

using instruction = std::uint8_t;

static instruction encode_instr(char color, char action)
{
    if (!action)
    {
        return 0;
    }
    int index = std::string("HWSENT").find_first_of(action) + 1;
    return (index << 5) | color;
}

static instruction program[MAX_STATE];

void set_instruction(std::vector<int> S, int Z, char A)
{
    if (S.size() != 5)
    {
        die("Invalid array");
    }
    for (int s : S)
    {
        if (s < -2 || s > MAX_COLOR)
        {
            die("Invalid array");
        }
    }
    if (Z < 0 || Z > MAX_COLOR)
    {
        die("Invalid color");
    }
    if (std::string("HWSENT").find_first_of(A) == std::string::npos)
    {
        die("Invalid action");
    }
    int code = encode_state(S);
    if (program[code])
    {
        die("Same state array");
    }
    program[code] = encode_instr(Z, A);
}

int main(int argc, char *argv[])
{
    assert(argc >= 3);
    output.open(argv[2], std::ios::binary);
    program_pulibot();
    output << output_secret << "\n";
    output << "OK\n";
    for (instruction instr : program)
    {
        output.put(instr);
    }
    return 0;
}
