#include "overtaking.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>
#include <array>
#include <map>

#define xx first
#define yy second

using namespace std;
using ll = long long;

int L, N, X, M;
vector<long long> T;
vector<int> W, S;
vector<vector<long long>> tm;
vector<vector<int>> ord;

pair<long long, int> get(long long Y);

vector<array<long long, 3>> lst;
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

    long long L = 0;
    while (L < (1LL << 60))
    {
        if (get(L).xx == get(L + 1).xx)
        {
            long long R = L;
            for (long long j = 60; j >= 0; j--)
            {
                long long r = R + (1LL << j);
                if (r < (1LL << 60) && get(r).xx == get(L).xx)
                {
                    R = r;
                }
            }
            lst.push_back({L, R, 0});
            L = R + 1;
        }
        else if (get(L).xx + 1 == get(L + 1).xx)
        {
            long long R = L;
            for (long long j = 60; j >= 0; j--)
            {
                long long r = R + (1LL << j);
                if (r < (1LL << 60) && get(r).xx + 1 == get(r + 1).xx && get(L).xx + r - L == get(r).xx && get(L).yy == get(r).yy)
                {
                    R = r;
                }
            }
            lst.push_back({L, R, 1});
            L = R + 1;
        }
        else
        {
            lst.push_back({L, L, 0});
            L++;
        }
    }

    return;
}

map<long long, pair<long long, int>> dp;
pair<long long, int> get(long long Y)
{
    if (dp.count(Y))
        return dp[Y];
    ll prev = Y;
    int sum = 0;
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
        sum += x;
        prev = res;
    }

    return dp[Y] = {prev, sum};
}

long long arrival_time(long long Y)
{
    auto it = lower_bound(lst.begin(), lst.end(), array<ll, 3>{Y, 0LL, 0LL});
    while (it == lst.end() || Y < (*it)[0])
        it = prev(it);
    if ((*it)[2] == 0)
        return get((*it)[0]).xx;
    return get((*it)[0]).xx + Y - (*it)[0];
}
