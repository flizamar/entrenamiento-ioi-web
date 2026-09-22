#include "robot.h"
#include <set>

const int ANY = -3;

std::set<std::vector<int>> prog;

void set(std::vector<int> S, int Z, char A)
{
    for (int i = 0; i < 5; i++)
    {
        if (S[i] == ANY)
        {
            auto S2 = S;
            for (int j = -2; j <= 3; j++)
            {
                S2[i] = j;
                set(S2, Z, A);
            }
            return;
        }
    }
    if (prog.count(S))
        return;
    prog.insert(S);
    set_instruction(S, Z, A);
}

void program_pulibot()
{
    set({ANY, -2, 1, ANY, -2}, 1, 'T');
    set({ANY, -2, ANY, 1, -2}, 1, 'T');
    set({ANY, ANY, 1, ANY, 2}, 1, 'N');
    set({ANY, ANY, ANY, 1, 2}, 1, 'N');
    set({ANY, ANY, -2, -2, 2}, 1, 'N');
    set({ANY, 3, 1, ANY, ANY}, 1, 'W');
    set({ANY, 3, ANY, 1, ANY}, 1, 'W');
    set({ANY, 3, -2, -2, ANY}, 1, 'W');
    set({0, ANY, 0, ANY, ANY}, 2, 'S');
    set({0, ANY, ANY, 0, ANY}, 3, 'E');
    set({2, ANY, ANY, 0, ANY}, 3, 'E');
    set({ANY, ANY, ANY, ANY, 2}, 0, 'N');
    set({ANY, 3, ANY, ANY, ANY}, 0, 'W');
}
