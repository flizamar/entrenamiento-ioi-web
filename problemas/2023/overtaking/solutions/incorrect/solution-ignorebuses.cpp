#include "overtaking.h"

#include <vector>
#include <algorithm>
#include <iostream>

#define MAXNM 1001000
#define MAXTIME 1000000000

using namespace std;

long long L, X;

void init(int LL, int /*NN*/, std::vector<long long> /*TT*/, std::vector<int> /*WW*/, int XX, int /*MM*/, std::vector<int> /*SS*/)
{
    L = LL;
    X = XX;
}

long long arrival_time(long long Y)
{
    return Y + L * X;
}