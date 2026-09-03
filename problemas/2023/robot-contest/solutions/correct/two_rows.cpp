#include "robot.h"

const int ANY = -3;

void set(std::vector<int> S, int Z, char A)
{
    for (int i = 0; i < 5; i++)
    {
        if (S[i] == ANY)
        {
            auto S2 = S;
            for (int j = -2; j <= 1; j++)
            {
                S2[i] = j;
                set(S2, Z, A);
            }
            return;
        }
    }
    set_instruction(S, Z, A);
}

void program_pulibot()
{
    set({0, ANY, ANY, 0, ANY}, 1, 'E');
    set({0, ANY, 0, -1, -2}, 1, 'S');
    set({0, ANY, 0, -2, -2}, 1, 'S');
    set({0, ANY, -2, -1, 0}, 1, 'N');
    set({0, ANY, -2, -2, ANY}, 1, 'T');
}
