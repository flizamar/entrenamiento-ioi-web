/* colors.txt:
#ffffff 0
#0060ff 1
#ff0000 ^
#ff0000 <
#005500 *
*/

#include "robot.h"

#include <map>
#include <algorithm>

#define ANY -3
#define UP 2
#define LEFT 3
#define BAD 4

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

    instruct({0, UP, ANY, ANY, ANY}, LEFT, 'H');
    instruct({0, LEFT, ANY, ANY, ANY}, LEFT, 'H');
    instruct({0, ANY, ANY, ANY, UP}, UP, 'H');
    instruct({0, ANY, ANY, ANY, LEFT}, UP, 'H');

    instruct({0, -2, ANY, ANY, -2}, UP, 'H');

    instruct({UP, ANY, ANY, ANY, ANY}, BAD, 'N');
    instruct({LEFT, ANY, ANY, ANY, ANY}, BAD, 'W');

    instruct({UP, ANY, 0, ANY, ANY}, UP, 'S');
    instruct({UP, ANY, ANY, 0, ANY}, UP, 'E');
    instruct({LEFT, ANY, 0, ANY, ANY}, LEFT, 'S');
    instruct({LEFT, ANY, ANY, 0, ANY}, LEFT, 'E');

    instruct({UP, ANY, 1, ANY, ANY}, 1, 'N');
    instruct({UP, ANY, ANY, 1, ANY}, 1, 'N');
    instruct({LEFT, ANY, 1, ANY, ANY}, 1, 'W');
    instruct({LEFT, ANY, ANY, 1, ANY}, 1, 'W');

    instruct({UP, ANY, -2, -2, ANY}, 1, 'N');
    instruct({LEFT, ANY, -2, -2, ANY}, 1, 'W');

    instruct({UP, -2, 1, ANY, -2}, 1, 'T');
    instruct({UP, -2, ANY, 1, -2}, 1, 'T');
    instruct({LEFT, -2, 1, ANY, -2}, 1, 'T');
    instruct({LEFT, -2, ANY, 1, -2}, 1, 'T');

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