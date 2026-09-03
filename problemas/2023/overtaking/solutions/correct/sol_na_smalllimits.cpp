#include "overtaking.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>
#include <array>
#include <map>

using namespace std;
using ll = long long;

int L, N, X, M;
vector<long long> T;
vector<int> W, S;
vector<vector<long long>> tm;
vector<vector<int>> ord;
void init(int L_, int N_, std::vector<long long> T_, std::vector<int> W_, int X_, int M_, std::vector<int> S_)
{
    L = L_;
    N = N_;
    T = T_;
    W = W_;
    X = X_;
    M = M_;
    S = S_;

    tm.assign(M, vector<long long>(N));
    for (int i = 0; i < N; ++i)
    {
        tm[0][i] = T[i];
    }

    ord.resize(M);
    for (int j = 0; j < N; ++j)
        ord[0].push_back(j);

    sort(ord[0].begin(), ord[0].end(), [&](int x, int y) -> bool
         {		
		if(tm[0][x]==tm[0][y]) return W[x]<W[y];
		return tm[0][x]<tm[0][y]; });

    for (int i = 1; i < M; ++i)
    {

        vector<array<ll, 3>> lst; //{start, en, ind}
        for (int j = 0; j < N; ++j)
        {
            lst.push_back({tm[i - 1][j], tm[i - 1][j] + (ll)W[j] * ll(S[i] - S[i - 1]), j});
        }

        sort(lst.begin(), lst.end(), [&](auto &x, auto &y) -> bool
             {
			if(x[0]==y[0]) return W[x[2]]>W[y[2]];
			return x[0]<y[0]; });

        ll prev = 0;
        for (int j = 0; j < N;)
        {
            int k;
            ll curr = 0;
            for (k = j; k < N && lst[j][0] == lst[k][0]; k++)
            {
                lst[k][1] = max(lst[k][1], prev);
                curr = max(curr, lst[k][1]);
            }

            prev = max(curr, prev);

            j = k;
        }

        for (int j = 0; j < N; ++j)
        {
            tm[i][lst[j][2]] = lst[j][1];
        }

        for (int j = 0; j < N; ++j)
            ord[i].push_back(j);
        sort(ord[i].begin(), ord[i].end(), [&](int x, int y) -> bool
             {
			if(tm[i][x]==tm[i][y]) return W[x]<W[y];
			return tm[i][x]<tm[i][y]; });
    }

    return;
}

map<long long, long long> dp;
long long get(long long Y)
{
    if (dp.count(Y))
        return dp[Y];
    ll prev = Y;
    for (int i = 1; i < M; ++i)
    {
        int x = -1;
        for (int j = 10; j >= 0; j--)
        {
            int xx = x + (1 << j);
            if (xx < N && tm[i - 1][ord[i - 1][xx]] < prev)
                x = xx;
        }
        ll res = prev + ll(S[i] - S[i - 1]) * X;
        if (x != -1)
            res = max(res, tm[i][ord[i - 1][x]]);

        prev = res;
    }

    return dp[Y] = prev;
}

long long arrival_time(long long Y)
{
    return get(Y);
}
