/* colors.txt:
#ffffff 0
#005ca5 1
#ff0000 a
#ff5500 b
#ffff00 c
*/

#include "robot.h"

const int next[5] = {-1, -1, 3, 4, 2};
const int prev[5] = {-1, -1, 4, 2, 3};
const char move[5] = {'_', 'W', 'S', 'E', 'N'};

bool isdist(int x)
{
    return 2 <= x;
}

void get_move(std::vector<int> S, int &Z, char &A)
{
    bool is_start = S[1] == -2 && S[4] == -2;
    bool is_end = S[2] == -2 && S[3] == -2;
    int dir = 0;
    for (int i = 1; i <= 4; i++)
    {
        if (isdist(S[i]))
        {
            dir = i;
        }
    }
    for (int i = 1; i <= 4; i++)
    {
        if (S[i] == prev[S[dir]])
        {
            dir = i;
        }
    }
    if (S[0] == 0)
    {
        if (is_start)
        {
            A = 'H';
            Z = 2;
        }
        else if (dir)
        {
            A = move[dir];
            Z = S[dir];
        }
    }
    else if (S[0] == 1)
    {
        if (!dir)
            return;
        bool done = true;
        int path = 0;
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == 1)
            {
                path = i;
            }
            else if (S[i] >= 2 && S[i] != S[dir])
            {
                done = false;
            }
        }
        if (done && path)
        {
            A = move[path];
            Z = prev[S[dir]];
        }
        else
        {
            A = move[dir];
            Z = 1;
        }
    }
    else if (isdist(S[0]))
    {
        if (is_end)
        {
            A = 'T';
            Z = 1;
            return;
        }
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == S[0])
            {
                A = 'H';
                Z = next[S[0]];
                return;
            }
        }
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == 0)
            {
                A = move[i];
                Z = prev[S[0]];
                return;
            }
        }
        if (is_start)
        {
            A = 'H';
            Z = 1;
            return;
        }
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == next[S[0]])
            {
                A = move[i];
                Z = 1;
                return;
            }
        }
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == 1)
            {
                A = move[i];
                Z = next[S[0]];
                return;
            }
        }
    }
}

void program_pulibot()
{
    std::vector<int> S(5);
    #define LOOP(i) for (S[i] = -2; S[i] < 5; S[i]++)
    LOOP(0) LOOP(1) LOOP(2) LOOP(3) LOOP(4)
    {
        int Z = -1;
        char A;
        get_move(S, Z, A);
        if (Z != -1)
            set_instruction(S, Z, A);
    }
}
