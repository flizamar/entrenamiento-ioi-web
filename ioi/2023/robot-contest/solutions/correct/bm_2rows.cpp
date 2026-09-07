#include "robot.h"

#define ANY -3

int colorMax = 1;

std::pair<int, int> range(int x)
{
    if (x == -3)
        return {-2, colorMax};
    return {x, x};
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
                        set_instruction({i0, i1, i2, i3, i4}, Z, A);
                    }
                }
            }
        }
    }
}

void program_pulibot()
{
    instruct({ANY, ANY, ANY, 0, ANY}, 1, 'E');
    instruct({ANY, ANY, ANY, -1, 0}, 1, 'N');
    instruct({ANY, ANY, ANY, -1, -2}, 1, 'S');
    instruct({ANY, ANY, 0, -2, ANY}, 1, 'S');
    instruct({ANY, ANY, -2, -2, ANY}, 1, 'T');
}