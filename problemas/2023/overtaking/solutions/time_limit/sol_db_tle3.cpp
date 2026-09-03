#include "overtaking.h"
#include <bits/stdc++.h>
#define all(x) (x).begin(), (x).end()
#define lsb(x) (x) & (-x)
using namespace std;

const int maxN = 1001;
typedef long long ll;

int n, m, x, ord[maxN][maxN];
ll arrive[maxN][maxN], premax[maxN][maxN], mem[maxN][maxN];

vector <int> ind, w, s;
vector <ll> t, sortedT[maxN];

void init(int L, int N, std::vector<long long> T, std::vector<int> W, int X, int M, std::vector<int> S)
{
    n = N, m = M, x = X, t = sortedT[0] = T, w = W, s = S;

    vector <int> needed;
    for (int i = 0; i < N; ++i) {
        if (W[i] > x) {
            needed.push_back(i);
        }
    }
    n = N = needed.size();
    for (int i = 0; i < N; ++i) {
        W[i] = w[needed[i]];
        T[i] = t[needed[i]];
    }
    w = W, t = T;

    vector <int> inds;
    for (int i = 0; i < N; ++i) {
        arrive[0][i] = T[i];
        inds.push_back(i);
    }
    auto order_buses = [&](int j) {
        sort(all(inds), [&](int x, int y) {
            return arrive[j][x] < arrive[j][y];
        });
        for (int i = 0; i < N; ++i) {
            ord[j][i] = inds[i];
        }
    };
    for (int j = 1; j < M; ++j) {
        ll pre = 0, lpre = 0;
        order_buses(j - 1);
        for (int i = 0; i < N; ++i) {
            if (i && arrive[j - 1][inds[i]] != arrive[j - 1][inds[i - 1]]) {
                lpre = pre;
            }
            arrive[j][inds[i]] = max(arrive[j - 1][inds[i]] + 1ll * W[inds[i]] * (S[j] - S[j - 1]), lpre);
            premax[j][i] = pre = max(pre, arrive[j][inds[i]]);
            sortedT[j].push_back(arrive[j][inds[i]]);
        }
    }
    order_buses(M - 1);
    memset(mem, -1, sizeof mem);
}

ll calc(int j, ll y) {
    if (j == m - 1) {
        return y;
    }
    int index = lower_bound(all(sortedT[j]), y) - sortedT[j].begin();
    if (index < n && sortedT[j][index] == y && mem[j][index] != -1) {
        return mem[j][index];
    }
    auto countBelow = [](int j, ll y) {
        int l = 0;
        while (l < n && arrive[j][ord[j][l]] < y) {
            l++;
        }
        return l;
    };
    int below = countBelow(j, y), l = below? j + 1: m, r = m;
    while (l < r) {
        int q = l + r >> 1;
        if (arrive[q][ord[q][below - 1]] < y + 1ll * x * (s[q] - s[j])) {
            l = q + 1;
        } else {
            r = q;
        }
    }
    if (l == m) {
        return y + 1ll * x * (s[m - 1] - s[j]);
    }
    ll res = calc(l, premax[l][below - 1]);
    if (index < n && sortedT[j][index] == y) {
        mem[j][index] = res;
    }
    return res;
}

long long arrival_time(long long y)
{
    return calc(0, y);
}