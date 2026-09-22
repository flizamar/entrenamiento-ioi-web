/* colors.txt:
#ffffff 0
#005ca5 1
#005500 <
#005500 v
#005500 >
#005500 ^
#ff0000 *
*/

#include "robot.h"

#include <map>
#include <algorithm>

#define ANY -3
#define DIR -4

#define LINE 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define UP 5

#define BLOCK 6

int colorMax = 6;

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

    instruct({LINE, ANY, -2, -2, ANY}, LINE, 'T');

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