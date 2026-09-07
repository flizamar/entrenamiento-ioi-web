/* colors.txt:
#ffffff 0
#005ca5 1
#ff0000 a
#ff5500 b
#ffff00 c
#005500 a
#00ff00 b
#aaff7f c
*/


// Has a bug in the cleanup part

#include "robot.h"

int dist(int x)
{
    return x < 2 ? 0 : (x + 1) % 3 + 1;
}

int state(int x)
{
    return (x + 1) / 3;
}

int color(int dist, int state)
{
    return (dist + 2) % 3 + state * 3 - 1;
}

bool isnext(int a, int b)
{
    if (a < 2 || b < 2)
        return false;
    return (a + 1) % 3 == b % 3;
}

char move[5] = {'_', 'W', 'S', 'E', 'N'};

void get_move(std::vector<int> S, int &Z, char &A)
{
    bool is_start = S[1] == -2 && S[4] == -2;
    bool is_end = S[2] == -2 && S[3] == -2;
    bool path1 = is_start;
    bool path2 = is_end;
    for (int i = 1; i <= 4; i++)
    {
        if (state(S[i]) == 2)
        {
            path1 = true;
        }
        if (S[i] == 1 && (S[0] == 1 || state(S[0]) == 2))
        {
            path2 = true;
        }
    }
    bool path = path1 && path2;
    if (path)
    {
        for (int i = 1; i <= 4; i++)
        {
            if (state(S[i]) == 1)
            {
                Z = 1;
                A = move[i];
                return;
            }
        }
        if (is_start)
        {
            Z = 1;
            A = 'T';
            return;
        }
        for (int i = 1; i <= 4; i++)
        {
            if (state(S[i]) == 2)
            {
                Z = 1;
                A = move[i];
                return;
            }
        }
    }
    if (S[0] == 0)
    {
        for (int i = 1; i <= 4; i++)
        {
            if (state(S[i]) == 2)
            {
                Z = color(dist(S[i]) - 1, 1);
                A = move[i];
                return;
            }
        }
        if (is_start)
        {
            Z = color(1, 1);
            A = 'H';
        }
        return;
    }
    bool cleanup = false;
    for (int i = 1; i <= 4; i++)
    {
        if (S[i] == 1 ||
            state(S[0]) == 2 ||
            dist(S[i]) == dist(S[0]))
        {
            cleanup = true;
        }
    }
    for (int i = 1; i <= 4; i++)
    {
        if (state(S[0]) == 2 && state(S[i]) == 1 && isnext(S[i], S[0]))
        {
            cleanup = false;
        }
    }
    if (cleanup)
    {
        for (int i = 1; i <= 4; i++)
        {
            if (state(S[i]) == 1 && ((state(S[0]) == 1 && isnext(S[0], S[i])) ||
                                     (state(S[0]) == 2 && dist(S[0]) == dist(S[i]))))
            {
                Z = color(dist(S[i]), 2);
                A = move[i];
                return;
            }
        }
        for (int i = 1; i <= 4; i++)
        {
            if (S[i] == 1)
            {
                Z = 0;
                A = move[i];
                return;
            }
            if (state(S[i]) == 2 && ((state(S[0]) == 2 && isnext(S[i], S[0])) ||
                                     (state(S[0]) == 1 && dist(S[0]) == dist(S[i]))))
            {
                Z = 0;
                A = move[i];
                return;
            }
        }
    }
    for (int i = 1; i <= 4; i++)
    {
        if (S[i] == 0)
        {
            Z = color(dist(S[0]), 2);
            A = move[i];
            return;
        }
        else if (isnext(S[0], S[i]))
        {
            Z = color(dist(S[0]), 2);
            A = move[i];
            return;
        }
    }
    Z = color(dist(S[0]) + 1, 1);
    A = 'H';
    for (int i = 1; i <= 4; i++)
    {
        if (state(S[i]) == 2)
        {
            A = move[i];
        }
    }
}

void program_pulibot()
{
    std::vector<int> S(5);
    #define LOOP(i) for (S[i] = -2; S[i] < 8; S[i]++)
    LOOP(0) LOOP(1) LOOP(2) LOOP(3) LOOP(4)
    {
        int Z = -1;
        char A;
        get_move(S, Z, A);
        if (Z != -1)
            set_instruction(S, Z, A);
    }
}
