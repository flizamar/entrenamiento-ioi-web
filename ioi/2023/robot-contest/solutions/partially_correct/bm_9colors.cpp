/* colors.txt:
#ffffff 0
#0060ff 1
#005500 <
#005500 v
#005500 >
#005500 ^
#ff0000 L
#ffff00 B
#ff5500 C
*/

#include "robot.h"

#include <map>
#include <algorithm>

#define ANY -3
#define DIR -4

#define LINE 6
#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define UP 5

#define BLOCK 7
#define CLEAR 8

int colorMax = 8;

std::map<int, std::pair<int, char>> insts;

std::pair<int, int> range(int x)
{
    if (x == ANY)
        return {-2, colorMax};
    if (x == DIR)
        return {LEFT, UP};
    // if(x==ANY0) return {LEFT0, UP0};
    // if(x==ANY1) return {LEFT1, UP1};
    return {x, x};
}

void map_instruction(const std::vector<int> &vec, int Z, char A)
{
    int p = 0;
    for (int i : vec)
        p = 30 * p + i + 2;
    insts[p] = {Z, A};
}

void instruct(const std::vector<int> &vec, int Z, char A)
{
    for (int i0 = range(vec[0]).first; i0 <= range(vec[0]).second; i0++)
    {
        for (int i1 = range(vec[1]).first; i1 <= range(vec[1]).second; i1++)
        {
            for (int i2 = range(vec[2]).first; i2 <= range(vec[2]).second; i2++)
            {
                for (int i3 = range(vec[3]).first; i3 <= range(vec[3]).second; i3++)
                {
                    for (int i4 = range(vec[4]).first; i4 <= range(vec[4]).second; i4++)
                    {
                        map_instruction({i0, i1, i2, i3, i4}, Z, A);
                    }
                }
            }
        }
    }
}

void program_pulibot()
{

    instruct({BLOCK, -2, ANY, ANY, -2}, LINE, 'H');

    instruct({BLOCK, LINE, ANY, ANY, ANY}, 0, 'W');
    instruct({BLOCK, ANY, LINE, ANY, ANY}, 0, 'S');
    instruct({BLOCK, ANY, ANY, LINE, ANY}, 0, 'E');
    instruct({BLOCK, ANY, ANY, ANY, LINE}, 0, 'N');

    instruct({BLOCK, 0, ANY, ANY, ANY}, BLOCK, 'W');
    instruct({BLOCK, ANY, 0, ANY, ANY}, BLOCK, 'S');
    instruct({BLOCK, ANY, ANY, 0, ANY}, BLOCK, 'E');
    instruct({BLOCK, ANY, ANY, ANY, 0}, BLOCK, 'N');

    instruct({LINE, ANY, ANY, ANY, ANY}, BLOCK, 'H');

    instruct({LINE, RIGHT, ANY, ANY, ANY}, LINE, 'W');
    instruct({LINE, ANY, UP, ANY, ANY}, LINE, 'S');
    instruct({LINE, ANY, ANY, LEFT, ANY}, LINE, 'E');
    instruct({LINE, ANY, ANY, ANY, DOWN}, LINE, 'N');

    instruct({DIR, LINE, ANY, ANY, ANY}, LINE, 'H');
    instruct({DIR, ANY, LINE, ANY, ANY}, LINE, 'H');
    instruct({DIR, ANY, ANY, LINE, ANY}, LINE, 'H');
    instruct({DIR, ANY, ANY, ANY, LINE}, LINE, 'H');

    instruct({0, BLOCK, ANY, ANY, ANY}, LEFT, 'W');
    instruct({0, ANY, BLOCK, ANY, ANY}, DOWN, 'S');
    instruct({0, ANY, ANY, BLOCK, ANY}, RIGHT, 'E');
    instruct({0, ANY, ANY, ANY, BLOCK}, UP, 'N');

    instruct({0, -2, ANY, ANY, -2}, LINE, 'H');

    instruct({0, LINE, ANY, ANY, ANY}, LEFT, 'W');
    instruct({0, ANY, LINE, ANY, ANY}, DOWN, 'S');
    instruct({0, ANY, ANY, LINE, ANY}, RIGHT, 'E');
    instruct({0, ANY, ANY, ANY, LINE}, UP, 'N');

    instruct({LINE, 1, ANY, ANY, ANY}, CLEAR, 'H');
    instruct({LINE, ANY, 1, ANY, ANY}, CLEAR, 'H');
    instruct({LINE, ANY, ANY, 1, ANY}, CLEAR, 'H');
    instruct({LINE, ANY, ANY, ANY, 1}, CLEAR, 'H');

    instruct({LINE, 1, 0, ANY, ANY}, LINE, 'S');
    instruct({LINE, 1, ANY, 0, ANY}, LINE, 'E');
    instruct({LINE, 1, ANY, ANY, 0}, LINE, 'N');

    instruct({LINE, 0, 1, ANY, ANY}, LINE, 'W');
    instruct({LINE, ANY, 1, 0, ANY}, LINE, 'E');
    instruct({LINE, ANY, 1, ANY, 0}, LINE, 'N');

    instruct({LINE, 0, ANY, 1, ANY}, LINE, 'W');
    instruct({LINE, ANY, 0, 1, ANY}, LINE, 'S');
    instruct({LINE, ANY, ANY, 1, 0}, LINE, 'N');

    instruct({LINE, 0, ANY, ANY, 1}, LINE, 'W');
    instruct({LINE, ANY, 0, ANY, 1}, LINE, 'S');
    instruct({LINE, ANY, ANY, 0, 1}, LINE, 'E');

    instruct({1, LINE, ANY, ANY, ANY}, 1, 'W');
    instruct({1, ANY, LINE, ANY, ANY}, 1, 'S');
    instruct({1, ANY, ANY, LINE, ANY}, 1, 'E');
    instruct({1, ANY, ANY, ANY, LINE}, 1, 'N');

    instruct({CLEAR, ANY, ANY, ANY, ANY}, 1, 'H');

    instruct({CLEAR, CLEAR, ANY, ANY, ANY}, 0, 'W');
    instruct({CLEAR, ANY, CLEAR, ANY, ANY}, 0, 'S');
    instruct({CLEAR, ANY, ANY, CLEAR, ANY}, 0, 'E');
    instruct({CLEAR, ANY, ANY, ANY, CLEAR}, 0, 'N');

    instruct({DIR, CLEAR, ANY, ANY, ANY}, CLEAR, 'H');
    instruct({DIR, ANY, CLEAR, ANY, ANY}, CLEAR, 'H');
    instruct({DIR, ANY, ANY, CLEAR, ANY}, CLEAR, 'H');
    instruct({DIR, ANY, ANY, ANY, CLEAR}, CLEAR, 'H');

    instruct({CLEAR, RIGHT, ANY, ANY, ANY}, CLEAR, 'W');
    instruct({CLEAR, ANY, UP, ANY, ANY}, CLEAR, 'S');
    instruct({CLEAR, ANY, ANY, LEFT, ANY}, CLEAR, 'E');
    instruct({CLEAR, ANY, ANY, ANY, DOWN}, CLEAR, 'N');

    instruct({LINE, ANY, -2, -2, ANY}, CLEAR, 'H');

    instruct({1, -2, ANY, ANY, -2}, 1, 'T');

    std::vector<int> vec;
    for (std::pair<int, std::pair<int, char>> i : insts)
    {
        vec.clear();
        for (int j = 0; j < 5; j++)
        {
            vec.push_back(i.first % 30 - 2);
            i.first /= 30;
        }
        std::reverse(vec.begin(), vec.end());
        set_instruction(vec, i.second.first, i.second.second);
    }
}