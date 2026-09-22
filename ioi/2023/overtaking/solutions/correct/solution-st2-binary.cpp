#include "overtaking.h"

#include <vector>
#include <algorithm>
#include <iostream>

#define MAXNM 1001000
#define MAXTIME 1000000000000000000

using namespace std;

long long a, X, N, M, L, Y, finishTime;
int cars[MAXNM];
long long T[MAXNM];
long long W[MAXNM];
long long S[MAXNM];
long long finish[MAXNM];

bool ahead(int x, int y)
{
    return T[x] < T[y] || (T[x] == T[y] && W[x] < W[y]);
}

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

    std::sort(cars, cars + N, ahead);
    finishTime = 0;
    for (int i = 0; i < N; i++)
    {
        int car = cars[i];
        finishTime = max(finishTime, T[car] + L * W[car]);
        finish[car] = finishTime;
    }
}

int binSearch(long long l, long long r)
{
    if (l == r)
        return cars[l];

    long long half = (l + r + 1) / 2;
    if (Y > T[cars[half]] || (Y == T[cars[half]] && X > W[cars[half]]))
        return binSearch(half, r);
    return binSearch(l, half - 1);
}

long long arrival_time(long long YY)
{
    Y = YY;

    if (Y < T[cars[0]] || (Y == T[cars[0]] && X < W[cars[0]]))
        return Y + L * X;

    return max(Y + L * X, finish[binSearch(0, N - 1)]);
}