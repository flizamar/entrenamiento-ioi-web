#include "overtaking.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>
#include <array>
#include <map>
#include <unordered_map>
#define xx first
#define yy second
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

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
    ord[0].resize(N);
    for (int j = 0; j < N; ++j)
        ord[0][j] = j;

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

        ord[i].resize(N);
        for (int j = 0; j < N; ++j)
            ord[i][j] = j;
        sort(ord[i].begin(), ord[i].end(), [&](int x, int y) -> bool
             {
			if(tm[i][x]==tm[i][y]) return W[x]<W[y];
			return tm[i][x]<tm[i][y]; });
    }

    return;
}

#include <ext/pb_ds/assoc_container.hpp>
__gnu_pbds::gp_hash_table<ll, ll> dp[1001];
vector<pair<ll, ll>> poss;
long long get(long long Y)
{
    ll prev = Y;
    poss.clear();
    for (int i = 1; i < M; ++i)
    {
        int x = -1;
        if (tm[i - 1][ord[i - 1][0]] < prev)
        {
            int L = -1, R = N - 1;
            while (L < R)
            {
                int mid = (L + R + 1) / 2;
                if (tm[i - 1][ord[i - 1][mid]] < prev)
                    L = mid;
                else
                    R = mid - 1;
            }
            x = L;
        }

        ll res = prev + ll(S[i] - S[i - 1]) * X;

        if (x != -1)
        {
            if (res <= tm[i][ord[i - 1][x]])
            {
                res = max(res, tm[i][ord[i - 1][x]]);
                auto it = dp[i].find(res);
                if (it != dp[i].end())
                {
                    prev = it->yy;
                    break;
                }

                prev = res;
                poss.push_back({i, res});
                continue;
            }
            else
            {
                int L = i, R = M - 1;
                while (L < R)
                {
                    int mid = (L + R + 1) / 2;
                    if (tm[mid][ord[mid - 1][x]] < prev + ll(S[mid] - S[i - 1]) * X)
                        L = mid;
                    else
                        R = mid - 1;
                }

                int to = L;
                prev = prev + ll(S[to] - S[i - 1]) * X;
                i = to;

                continue;
            }
        }
        else
        {
            int L = i, R = M - 1;
            while (L < R)
            {
                int mid = (L + R + 1) / 2;
                if (tm[mid - 1][ord[mid - 1][0]] >= prev + ll(S[mid] - S[i - 1]) * X)
                    L = mid;
                else
                    R = mid - 1;
            }

            int to = L;
            prev = prev + ll(S[to] - S[i - 1]) * X;
            i = to;
            continue;
        }
    }
    for (auto &i : poss)
    {
        dp[i.xx][i.yy] = prev;
    }

    return prev;
}

long long arrival_time(long long Y)
{
    return get(Y);
}
