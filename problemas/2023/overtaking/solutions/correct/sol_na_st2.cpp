#include "overtaking.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>

using namespace std;
using ll = long long;

int L, N, X, M;
vector<long long> T;
vector<int> W, S;

vector<long long> dp;
vector<int> ord;

void init(int L_, int N_, std::vector<long long> T_, std::vector<int> W_, int X_, int M_, std::vector<int> S_)
{
    L = L_;
    N = N_;
    T = T_;
    W = W_;
    X = X_;
    M = M_;
    S = S_;

    T.push_back(-1);
    dp.assign(N, 0);
    for (int i = 0; i < N; ++i)
    {
        dp[i] = T[i] + ll(W[i]) * ll(S[1] - S[0]);
    }
    ord.assign(N, 0);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int x, int y) -> bool
         { return T[x] < T[y]; });

    for (int i = 1; i < N; ++i)
    {
        if (dp[ord[i - 1]] > dp[ord[i]])
        {
            dp[ord[i]] = dp[ord[i - 1]];
        }
    }

    return;
}

long long arrival_time(long long Y)
{
    int x = -1;
    for (int j = 10; j >= 0; j--)
    {
        int xx = x + (1 << j);
        if (xx < N && T[ord[xx]] < Y)
            x = xx;
    }
    ll base = Y + (ll)X * ll(S[1] - S[0]);
    if (x == -1)
        return base;

    return max(base, dp[ord[x]]);
}