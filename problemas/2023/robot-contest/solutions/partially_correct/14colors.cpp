/* colors.txt:
#ffffff 0
#005ca5 1
#005500 <
#005500 v
#005500 >
#005500 ^
#ff0000 <
#ff0000 v
#ff0000 >
#ff0000 ^
#ffff00 <
#ffff00 v
#ffff00 >
#ffff00 ^
*/

#include "robot.h"

int dir(int x)
{
    if (x < 2)
        return 0;
    return (x - 2) % 4 + 1;
}

int state(int x)
{
    if (x < 2)
        return 0;
    return (x - 2) / 4 + 1;
}

int color(int dir, int state)
{
    return state * 4 + dir - 3;
}

char move[5] = {'_', 'W', 'S', 'E', 'N'};

int opp[] = {0, 3, 4, 1, 2};

void get_move(std::vector<int> S, int &Z, char &A)
{
    bool is_start = S[1] == -2 && S[4] == -2;
    bool is_end = S[2] == -2 && S[3] == -2;
    if (S[0] == 0)
    {
        for (int i = 1; i <= 4; i++)
        {
            if (state(S[i]) == 3)
            {
                Z = is_end ? 1 : color(i, 1);
                A = move[i];
                return;
            }
        }
        if (is_start)
        {
            Z = color(1, 3);
            A = 'H';
        }
        return;
    }
    if (dir(S[0]) == 0)
        return;
    bool cleanup = false;
    bool path = is_end;
    for (int i = 1; i <= 4; i++)
    {
        if (S[i] == 1)
        {
            path = true;
        }
        else if (state(S[i]) == 3)
        {
            cleanup = true;
        }
    }
    if (cleanup || path)
    {
        for (int i = 1; i <= 4; i++)
        {
            if (dir(S[i]) == opp[i])
            {
                Z = color(dir(S[0]), 3);
                A = move[i];
                return;
            }
        }
        Z = path && !cleanup;
        A = is_start ? 'T' : move[dir(S[0])];
        return;
    }
    int s = is_start ? state(S[0]) == 3 ? 1 : state(S[0]) : state(S[dir(S[0])]);
    if (s != 1 && s != 2)
        return;
    for (int i = 1; i <= 4; i++)
    {
        if (s == 1 && S[i] == 0)
        {
            Z = color(dir(S[0]), 3);
            A = move[i];
            return;
        }
        if (dir(S[i]) == opp[i] && state(S[i]) != s)
        {
            Z = color(dir(S[0]), s);
            A = move[i];
            return;
        }
    }
    if (is_start)
    {
        Z = color(1, 3 - s);
        A = 'H';
    }
    else
    {
        Z = color(dir(S[0]), s);
        A = move[dir(S[0])];
    }
}

void program_pulibot()
{
    std::vector<int> S(5);
    #define LOOP(i) for (S[i] = -2; S[i] < 14; S[i]++)
    LOOP(0) LOOP(1) LOOP(2) LOOP(3) LOOP(4)
    {
        int Z = -1;
        char A;
        get_move(S, Z, A);
        if (Z != -1)
            set_instruction(S, Z, A);
    }
}
