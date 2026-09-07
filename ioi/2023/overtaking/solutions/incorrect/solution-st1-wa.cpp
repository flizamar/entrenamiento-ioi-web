#include "overtaking.h"

#include <vector>
#include <algorithm>
#include <iostream>

#define MAXNM 1001000
#define MAXTIME 1000000000000000000

using namespace std;

long long X, N, L;
long long T[MAXNM];
long long W[MAXNM];

void init(int LL, int NN, std::vector<long long> TT, std::vector<int> WW, int XX, int /*MM*/, std::vector<int> /*SS*/)
{
    L = LL;
    N = NN;
    X = XX;
    int id = 0;
    for (long long i : TT)
        T[id++] = i;
    id = 0;
    for (int i : WW)
        W[id++] = i;
}

long long arrival_time(long long Y)
{
    if (T[0] < Y)
        return std::max(Y + L * X, T[0] + L * W[0]);
    return Y + L * X;
}