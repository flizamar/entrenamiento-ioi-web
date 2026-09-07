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
            if (state(S[i]) == 2)
            {
                Z = is_end ? color(2, 2) : color(i, 1);
                A = move[i];
                return;
            }
        }
        if (is_start)
        {
            Z = color(1, 2);
            A = 'H';
        }
        return;
    }
    int stackdir = 0;
    int path = 0;
    int dir1 = 0;
    for (int i = 1; i <= 4; i++)
    {
        if (state(S[i]) == 2 && dir(S[i]) == opp[i])
        {
            stackdir = i;
        }
        else if (state(S[i]) == 2)
        {
            path = i;
        }
        else if (S[i] == 1)
        {
            dir1 = i;
        }
    }
    if (dir1 || path)
    {
        for (int i = 1; i <= 4; i++)
        {
            if (state(S[i]) == 1 && dir(S[i]) == opp[i])
            {
                Z = 1;
                A = move[i];
                return;
            }
        }
    }
    if (dir1 && is_end)
    {
        Z = 1;
        A = 'T';
        return;
    }
    if (path && ((dir1 && state(S[0]) == 2) || is_start))
    {
        Z = 1;
        A = move[path];
        return;
    }
    if (dir1)
    {
        Z = 0;
        A = move[dir1];
        return;
    }
    if (path && stackdir)
    {
        Z = color(path, 2);
        A = move[stackdir];
        return;
    }
    int last = state(S[0]) == 2 ? dir(S[0]) : stackdir;
    for (int i = 0; i < 4; i++)
    {
        int d = (last + i) % 4 + 1;
        if (d == stackdir)
        {
            Z = color(stackdir, 1);
            A = move[stackdir];
            return;
        }
        if (S[d] == 0 || (state(S[d]) == 1 && dir(S[d]) == opp[d]))
        {
            Z = color(d, 2);
            A = move[d];
            return;
        }
    }
}

void program_pulibot()
{
    std::vector<int> S(5);
    #define LOOP(i) for (S[i] = -2; S[i] < 10; S[i]++)
    LOOP(0) LOOP(1) LOOP(2) LOOP(3) LOOP(4)
    {
        int Z = -1;
        char A;
        get_move(S, Z, A);
        if (Z != -1)
            set_instruction(S, Z, A);
    }
}
