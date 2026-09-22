/* colors.txt:
#ffffff 0
#0060ff 1
#005500 <
#005500 v
#005500 >
#005500 ^
#ff0000 a
#ff0000 b
#ff0000 c
*/

#include "robot.h"

#include <map>
#include <algorithm>

#define ANY -3
#define DIR -4
#define EMPTY -5
#define LASTALL -6

#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define UP 5
#define LAST1 6
#define LAST2 7
#define LAST3 8

int colorMax = 8;

std::map<int, std::pair<int, char>> insts;

std::pair<int, int> range(int x)
{
    if (x == ANY)
        return {-2, colorMax};
    if (x == DIR)
        return {LEFT, UP};
    if (x == EMPTY)
        return {0, UP};
    if (x == LASTALL)
        return {LAST1, LAST3};
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

    instruct({LAST3, LASTALL, ANY, ANY, ANY}, LEFT, 'W');
    instruct({LAST2, LASTALL, ANY, ANY, ANY}, LAST3, 'H');
    instruct({LAST2, LASTALL, ANY, LEFT, ANY}, LAST3, 'E');
    instruct({LAST1, LASTALL, ANY, ANY, ANY}, LAST2, 'H');
    instruct({LAST1, LASTALL, ANY, ANY, DOWN}, LAST2, 'N');
    instruct({LEFT, LASTALL, ANY, ANY, ANY}, LAST1, 'H');
    instruct({LEFT, LASTALL, UP, ANY, ANY}, LAST1, 'S');

    instruct({LAST3, ANY, LASTALL, ANY, ANY}, DOWN, 'S');
    instruct({LAST2, ANY, LASTALL, ANY, ANY}, LAST3, 'H');
    instruct({LAST2, ANY, LASTALL, ANY, DOWN}, LAST3, 'N');
    instruct({LAST1, ANY, LASTALL, ANY, ANY}, LAST2, 'H');
    instruct({LAST1, RIGHT, LASTALL, ANY, ANY}, LAST2, 'W');
    instruct({DOWN, ANY, LASTALL, ANY, ANY}, LAST1, 'H');
    instruct({DOWN, ANY, LASTALL, LEFT, ANY}, LAST1, 'E');

    instruct({LAST3, ANY, ANY, LASTALL, ANY}, RIGHT, 'E');
    instruct({LAST2, ANY, ANY, LASTALL, ANY}, LAST3, 'H');
    instruct({LAST2, RIGHT, ANY, LASTALL, ANY}, LAST3, 'W');
    instruct({LAST1, ANY, ANY, LASTALL, ANY}, LAST2, 'H');
    instruct({LAST1, ANY, UP, LASTALL, ANY}, LAST2, 'S');
    instruct({RIGHT, ANY, ANY, LASTALL, ANY}, LAST1, 'H');
    instruct({RIGHT, ANY, ANY, LASTALL, DOWN}, LAST1, 'N');

    instruct({LAST3, ANY, ANY, ANY, LASTALL}, UP, 'N');
    instruct({LAST2, ANY, ANY, ANY, LASTALL}, LAST3, 'H');
    instruct({LAST2, ANY, UP, ANY, LASTALL}, LAST3, 'S');
    instruct({LAST1, ANY, ANY, ANY, LASTALL}, LAST2, 'H');
    instruct({LAST1, ANY, ANY, LEFT, LASTALL}, LAST2, 'E');
    instruct({UP, ANY, ANY, ANY, LASTALL}, LAST1, 'H');
    instruct({UP, RIGHT, ANY, ANY, LASTALL}, LAST1, 'W');

    instruct({0, LASTALL, ANY, ANY, ANY}, LEFT, 'W');
    instruct({0, ANY, LASTALL, ANY, ANY}, DOWN, 'S');
    instruct({0, ANY, ANY, LASTALL, ANY}, RIGHT, 'E');
    instruct({0, ANY, ANY, ANY, LASTALL}, UP, 'N');

    instruct({0, LAST1, -2, -2, ANY}, 1, 'W');
    instruct({0, LAST2, -2, -2, ANY}, 1, 'W');
    instruct({0, LAST3, -2, -2, ANY}, 1, 'W');
    instruct({0, ANY, -2, -2, LAST1}, 1, 'N');
    instruct({0, ANY, -2, -2, LAST2}, 1, 'N');
    instruct({0, ANY, -2, -2, LAST3}, 1, 'N');

    instruct({DIR, 0, ANY, ANY, ANY}, LAST3, 'H');
    instruct({DIR, ANY, 0, ANY, ANY}, LAST3, 'H');
    instruct({DIR, ANY, ANY, 0, ANY}, LAST3, 'H');
    instruct({DIR, ANY, ANY, ANY, 0}, LAST3, 'H');

    instruct({LAST3, 0, ANY, ANY, ANY}, LAST3, 'W');
    instruct({LAST3, ANY, 0, ANY, ANY}, LAST3, 'S');
    instruct({LAST3, ANY, ANY, 0, ANY}, LAST3, 'E');
    instruct({LAST3, ANY, ANY, ANY, 0}, LAST3, 'N');

    instruct({0, -2, ANY, ANY, -2}, LAST2, 'H');
    instruct({LAST2, -2, EMPTY, ANY, -2}, LAST3, 'S');
    instruct({LAST2, -2, ANY, EMPTY, -2}, LAST3, 'E');
    instruct({LAST3, -2, ANY, EMPTY, -2}, LAST2, 'E');
    instruct({LAST3, -2, EMPTY, ANY, -2}, LAST2, 'S');

    instruct({LAST1, 1, ANY, ANY, ANY}, 1, 'H');
    instruct({LAST1, ANY, 1, ANY, ANY}, 1, 'H');
    instruct({LAST1, ANY, ANY, 1, ANY}, 1, 'H');
    instruct({LAST1, ANY, ANY, ANY, 1}, 1, 'H');

    instruct({LAST2, 1, ANY, ANY, ANY}, 1, 'H');
    instruct({LAST2, ANY, 1, ANY, ANY}, 1, 'H');
    instruct({LAST2, ANY, ANY, 1, ANY}, 1, 'H');
    instruct({LAST2, ANY, ANY, ANY, 1}, 1, 'H');

    instruct({LAST3, 1, ANY, ANY, ANY}, 1, 'H');
    instruct({LAST3, ANY, 1, ANY, ANY}, 1, 'H');
    instruct({LAST3, ANY, ANY, 1, ANY}, 1, 'H');
    instruct({LAST3, ANY, ANY, ANY, 1}, 1, 'H');

    instruct({1, LAST1, ANY, ANY, ANY}, 1, 'W');
    instruct({1, ANY, LAST1, ANY, ANY}, 1, 'S');
    instruct({1, ANY, ANY, LAST1, ANY}, 1, 'E');
    instruct({1, ANY, ANY, ANY, LAST1}, 1, 'N');

    instruct({1, LAST2, ANY, ANY, ANY}, 1, 'W');
    instruct({1, ANY, LAST2, ANY, ANY}, 1, 'S');
    instruct({1, ANY, ANY, LAST2, ANY}, 1, 'E');
    instruct({1, ANY, ANY, ANY, LAST2}, 1, 'N');

    instruct({1, LAST3, ANY, ANY, ANY}, 1, 'W');
    instruct({1, ANY, LAST3, ANY, ANY}, 1, 'S');
    instruct({1, ANY, ANY, LAST3, ANY}, 1, 'E');
    instruct({1, ANY, ANY, ANY, LAST3}, 1, 'N');

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