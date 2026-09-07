#include "overtaking.h"

#include <vector>
#include <algorithm>
#include <iostream>

#define MAXNM 1001000
#define MAXTIME 1000000000000000000

using namespace std;

long long a, X, N, M, L, Y;
int cars[MAXNM];
long long T[MAXNM];
long long W[MAXNM];
long long S[MAXNM];

void init(int LL, int NN, std::vector<long long> TT, std::vector<int> WW, int XX, int MM, std::vector<int> SS)
{
    L = LL;
    N = NN;
    M = MM;
    X = XX;
    for (int i = 0; i < N; i++)
        cars[i] = i;
    int id = 0;
    for (long long i : TT)
        T[id++] = i;
    id = 0;
    for (int i : WW)
        W[id++] = i;
    id = 0;
    for (int i : SS)
        S[id++] = i;
}

int binSearch(long long l, long long r)
{
    if (l == r)
    {
        if (T[0] + W[0] * S[l] >= Y + X * S[l])
        {
            return l;
        }
        return -1;
    }

    long long half = (l + r) / 2;
    if (Y + X * S[half] <= T[0] + W[0] * S[half])
        return binSearch(l, half);
    return binSearch(half + 1, r);
}

long long arrival_time(long long YY)
{
    Y = YY;
    if (W[0] <= X || T[0] >= Y)
        return Y + X * L;

    int id = binSearch(1, M - 1);

    if (id == -1)
    {
        return Y + X * L;
    }

    return T[0] + W[0] * S[id] + (L - S[id]) * X;
}