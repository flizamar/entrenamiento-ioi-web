#include "robot.h"

char move[5] = {'_', 'W', 'S', 'E', 'N'};

void get_move(std::vector<int> S, int &Z, char &A)
{
    if (S[0] < 0)
    {
        return;
    }
    bool is_start = S[1] == -2 && S[4] == -2;
    bool is_end = S[2] == -2 && S[3] == -2;
    int neighbors = is_start + is_end;
    for (int i = 1; i <= 4; i++)
    {
        if (S[i] > 0)
        {
            neighbors++;
        }
    }
    if (neighbors >= 2)
    {
        if (is_start)
        {
            A = 'T';
            Z = 1;
            return;
        }
        for (int i = 1; i <= 4; i++)
        {
            if (i != S[0] && S[i] > 0)
            {
                A = move[i];
                Z = 1;
                return;
            }
        }
    }
    for (int i = S[0] + 1; i <= 4; i++)
    {
        if (S[i] == 0)
        {
            A = move[i];
            Z = i;
            return;
        }
    }
    for (int i = 1; i <= 4; i++)
    {
        if (S[i] > 0)
        {
            A = move[i];
            Z = 0;
            return;
        }
    }
}

void program_pulibot()
{
    std::vector<int> S(5);
    #define LOOP(i) for (S[i] = -2; S[i] <= 4; S[i]++)
    LOOP(0) LOOP(1) LOOP(2) LOOP(3) LOOP(4)
    {
        int Z = -1;
        char A;
        get_move(S, Z, A);
        if (Z != -1)
            set_instruction(S, Z, A);
    }
}
