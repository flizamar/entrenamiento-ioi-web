#include "tree.h"
#include <bits/stdc++.h>

#define mp make_pair
#define mt make_tuple
#define fi first
#define se second
#define pb push_back
#define sz(x) (int)((x).size())
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define for1(i, n) for (int i = 1; i <= (int)(n); ++i)
#define ford(i, n) for (int i = (int)(n) - 1; i >= 0; --i)
#define fore(i, a, b) for (int i = (int)(a); i <= (int)(b); ++i)

using namespace std;

typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vpi;
typedef vector<vi> vvi;
typedef long long i64;
typedef vector<i64> vi64;
typedef vector<vi64> vvi64;
typedef pair<i64, i64> pi64;
typedef double ld;

template<class T> bool uin(T &a, T b) { return a > b ? (a = b, true) : false; }
template<class T> bool uax(T &a, T b) { return a < b ? (a = b, true) : false; }

const int maxn = 310000;

i64 ans[maxn];
int n;
int p[maxn];
i64 w[maxn];
vi e[maxn];

int par[maxn], nleaves[maxn], tcr[maxn];
i64 sl[maxn];

int root(int v) {
    return v == par[v] ? v : par[v] = root(par[v]);
}

void init(vi P, vi W) {
    n = sz(P);
    copy(all(P), p);
    copy(all(W), w);
    forn(i, n) e[i].clear();
    for1(i, n - 1) e[P[i]].pb(i);

    vi ord(n);
    iota(all(ord), 0);
    sort(rall(ord), [&](int i, int j){return w[i] < w[j];});

    iota(par, par + n, 0);
    fill(nleaves, nleaves + n, 1);
    fill(tcr, tcr + n, n + 1);
    fill(sl, sl + n + 1, 0);
    i64 v0 = 0;

    for (int v: ord) {
        if (e[v].empty()) {
            v0 += w[v];
            continue;
        }
        int r = root(v);
        sl[nleaves[r]] += tcr[r] - w[v];
        --nleaves[r];
        tcr[r] = w[v];
        for (int u: e[v]) {
            sl[nleaves[u]] += tcr[u] - w[v];
            par[u] = r;
            nleaves[r] += nleaves[u];
        }
    }
    sl[nleaves[0]] += tcr[0];
    i64 slope = 0, v = v0;
    ford(i, n + 1) {
        v += slope;
        ans[i] = v;
        slope += sl[i];
    }
}

i64 query(int L, int R) {
    vvi64 dp(n, vi64(R - L + 1));
    auto vmerge = [](const vi64 &a, const vi64 &b) {
        vi64 c(sz(a) + sz(b) - 1, 1e18);
        forn(i, sz(a)) forn(j, sz(b)) uin(c[i + j], a[i] + b[j]);
        return c;
    };
    ford(v, n) {
        if (e[v].empty()) {
            forn(i, R - L + 1) dp[v][i] = (L + i) * w[v];
        } else {
            vi64 vdp = {0};
            for (int u: e[v]) {
                vdp = vmerge(vdp, dp[u]);
            }
            vi64 cdp(R * sz(e[v]) + 1, 1e18);
            forn(i, sz(vdp)) cdp[sz(e[v]) * L + i] = vdp[i];
            ford(i, sz(cdp) - 1) uin(cdp[i], cdp[i + 1] + w[v]);
            forn(i, sz(cdp) - 1) uin(cdp[i + 1], cdp[i] + w[v]);
            forn(j, R - L + 1) dp[v][j] = cdp[L + j];
        }
    }
    return *min_element(all(dp[0]));
}
