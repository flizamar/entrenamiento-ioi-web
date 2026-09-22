#include "overtaking.h"
#include <bits/stdc++.h>
#define all(x) (x).begin(), (x).end()
#define lsb(x) (x) & (-x)
using namespace std;

const int maxN = 1001;
typedef long long ll;

int n, m, x, ord[maxN][maxN];
ll arrive[maxN][maxN], premax[maxN][maxN];

vector <int> ind, w, s;
vector <ll> t;

void init(int L, int N, std::vector<long long> T, std::vector<int> W, int X, int M, std::vector<int> S)
{
    n = N, m = M, x = X, t = T, w = W, s = S;

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
        }
    }
    order_buses(M - 1);
}

long long arrival_time(long long y)
{
    for (int j = 1; j < m; ++j) {
        int l = 0;
        int r = n;
        while (l < r) {
            int q = l + r >> 1;
            if (arrive[j - 1][ord[j - 1][q]] < y) {
                l = q + 1;
            } else {
                r = q;
            }
        }
        y = max(y + 1ll * x * (s[j] - s[j - 1]), l? premax[j][l - 1]: 0);
    }
    return y;
}