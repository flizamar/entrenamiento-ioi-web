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
            for (int j = -2; j <= 2; j++)
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
    set({ANY, ANY, -2, -2, ANY}, 1, 'T');
    set({ANY, ANY, ANY, 0, ANY}, 1, 'E');
    set({ANY, ANY, 0, ANY, ANY}, 1, 'S');
    set({ANY, ANY, ANY, ANY, 1}, 2, 'N');
    set({ANY, 1, ANY, ANY, ANY}, 2, 'W');
}
