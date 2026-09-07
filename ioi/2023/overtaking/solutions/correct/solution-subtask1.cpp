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

long long arrival_time(long long YY)
{
    Y = YY;
    if (W[0] <= X)
        return Y + X * L;

    long long reserve = T[0];
    for (int s = 1; s < M; s++)
    {
        long long d = S[s] - S[s - 1];
        if (T[0] >= Y)
        {
            Y += X * d;
            T[0] += W[0] * d;
        }
        else
        {
            T[0] += W[0] * d;
            Y = max(Y + X * d, T[0]);
        }
    }
    T[0] = reserve;
    return Y;
}