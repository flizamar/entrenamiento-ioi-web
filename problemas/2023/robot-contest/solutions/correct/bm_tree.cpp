#include "robot.h"

#include <map>
#include <algorithm>

#define ANY -3

int colorMax = 4;

std::map<int, std::pair<int, char>> insts;

std::pair<int, int> range(int x)
{
    if (x == -3)
        return {-2, colorMax};
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
    instruct({0, ANY, ANY, ANY, ANY}, 2, 'H');

    instruct({2, 2, ANY, ANY, ANY}, 3, 'W');
    instruct({2, ANY, 2, ANY, ANY}, 3, 'S');
    instruct({2, ANY, ANY, 2, ANY}, 3, 'E');
    instruct({2, ANY, ANY, ANY, 2}, 3, 'N');

    instruct({2, 0, ANY, ANY, ANY}, 2, 'W');
    instruct({2, ANY, 0, ANY, ANY}, 2, 'S');
    instruct({2, ANY, ANY, 0, ANY}, 2, 'E');
    instruct({2, ANY, ANY, ANY, 0}, 2, 'N');

    instruct({2, 1, ANY, ANY, ANY}, 4, 'H');
    instruct({2, ANY, 1, ANY, ANY}, 4, 'H');
    instruct({2, ANY, ANY, 1, ANY}, 4, 'H');
    instruct({2, ANY, ANY, ANY, 1}, 4, 'H');

    instruct({2, ANY, -2, -2, ANY}, 4, 'H');

    instruct({3, ANY, ANY, ANY, ANY}, 4, 'H');

    instruct({4, ANY, ANY, ANY, ANY}, 1, 'H');

    instruct({4, 4, ANY, ANY, ANY}, 0, 'W');
    instruct({4, ANY, 4, ANY, ANY}, 0, 'S');
    instruct({4, ANY, ANY, 4, ANY}, 0, 'E');
    instruct({4, ANY, ANY, ANY, 4}, 0, 'N');

    instruct({4, 3, ANY, ANY, ANY}, 4, 'W');
    instruct({4, ANY, 3, ANY, ANY}, 4, 'S');
    instruct({4, ANY, ANY, 3, ANY}, 4, 'E');
    instruct({4, ANY, ANY, ANY, 3}, 4, 'N');

    instruct({1, ANY, ANY, ANY, ANY}, 1, 'T');

    instruct({1, 2, ANY, ANY, ANY}, 1, 'W');
    instruct({1, ANY, 2, ANY, ANY}, 1, 'S');
    instruct({1, ANY, ANY, 2, ANY}, 1, 'E');
    instruct({1, ANY, ANY, ANY, 2}, 1, 'N');

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