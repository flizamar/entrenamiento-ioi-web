/* colors.txt:
#ffffff 0
#005ca5 1
#ff0000 2
#005500 <
#005500 v
#005500 >
#005500 ^
*/

// See also BM_7colors_partial.cpp for a variant without the cleanup part

#include "robot.h"

const int PATH = 2;
const int RET = 1;

int dir[] = {-1, 3, 4, 5, 6};
int opp[] = {0, 3, 4, 1, 2};
char move[5] = {'_', 'W', 'S', 'E', 'N'};

void get_move(std::vector<int> S, int &Z, char &A)
{
    bool is_start = S[1] == -2 && S[4] == -2;
    bool is_end = S[2] == -2 && S[3] == -2;
    if (S[0] == 0)
    {
        if (is_start)
        {
            A = 'H';
            Z = RET;
            return;
        }
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == RET || S[i] == PATH)
            {
                A = move[i];
                Z = dir[i];
                return;
            }
        }
    }
    else if (S[0] == RET)
    {
        bool cleanup = false;
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == RET)
            {
                cleanup = true;
            }
        }
        if (cleanup)
        {
            Z = RET;
            for (int i = 1; i <= 4; i++)
            {
                if (S[i] == dir[opp[i]])
                {
                    A = move[i];
                    return;
                }
            }
            for (int i = 1; i <= 4; i++)
            {
                if (S[i] == PATH)
                {
                    A = move[i];
                    return;
                }
            }
            A = 'T';
            return;
        }
        else
        {
            for (int i = 1; i <= 4; i++)
            {
                if (S[i] == 0)
                {
                    A = move[i];
                    Z = RET;
                    return;
                }
            }
            if (is_start)
            {
                A = 'H';
                Z = PATH;
                return;
            }
            for (int i = 1; i <= 4; i++)
            {
                if (S[i] == PATH)
                {
                    A = move[i];
                    Z = 0;
                    return;
                }
            }
        }
    }
    else if (S[0] == PATH)
    {
        bool cleanup = false;
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == RET)
            {
                cleanup = true;
            }
        }
        if (cleanup)
        {
            for (int i = 1; i <= 4; i++)
            {
                if (S[i] == 0)
                {
                    A = move[i];
                    Z = PATH;
                    return;
                }
            }
        }
        else
        {
            for (int i = 1; i <= 4; i++)
            {
                if (S[i] == dir[opp[i]])
                {
                    A = move[i];
                    Z = PATH;
                    return;
                }
            }
        }
        A = 'H';
        Z = RET;
        return;
    }
    else if (S[0] >= 0)
    {
        int d = 0;
        for (int i = 1; i <= 4; i++)
        {
            if (S[0] == dir[i])
            {
                d = i;
            }
        }
        if (is_end)
        {
            A = move[d];
            Z = RET;
            return;
        }
        if (S[d] == PATH)
        {
            A = 'H';
            Z = PATH;
            return;
        }
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == dir[opp[i]])
            {
                A = move[i];
                Z = S[0];
                return;
            }
        }
        A = move[d];
        Z = 0;
        return;
    }
}

void program_pulibot()
{
    std::vector<int> S(5);
    #define LOOP(i) for (S[i] = -2; S[i] < 7; S[i]++)
    LOOP(0) LOOP(1) LOOP(2) LOOP(3) LOOP(4)
    {
        int Z = -1;
        char A;
        get_move(S, Z, A);
        if (Z != -1)
            set_instruction(S, Z, A);
    }
}
