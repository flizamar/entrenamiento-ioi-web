// In the name of God
#include "beechtree.h"

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

const int maxn = 2e5 + 100;

#define pb push_back
#define Mp make_pair
#define F first
#define S second
#define Sz(x) int((x).size())
#define all(x) (x).begin(), (x).end()

int n, m;
int a[maxn], p[maxn], sz[maxn];
vector<pii> g[maxn];
map<int, int> gsz[maxn];
set<pii> st[maxn];
bool ok[maxn];

void dfs(int v)
{
    sz[v] = 1;
    for (pii u : g[v])
    {
        dfs(u.F);
        sz[v] += sz[u.F];
        if (gsz[v].count(u.S))
            ok[v] = false;
        gsz[v][u.S] = sz[u.F];
    }
    return;
}

bool check(int v, int u)
{
    if (Sz(gsz[v]) > Sz(gsz[u]))
        return 0;
    for (pii i : gsz[v])
    {
        auto it = gsz[u].find(i.F);
        if (it == gsz[u].end() && i.S > it->S)
            return 0;
    }
    return 1;
}

bool add(int x, pii o)
{
    st[x].insert(o);

    auto it = st[x].find(o);

    if (it != st[x].begin())
    {
        auto it2 = it;
        it2--;
        if (!check(it2->S, o.S))
            return 0;
    }

    it++;

    if (it != st[x].end())
    {
        if (!check(o.S, it->S))
            return 0;
    }

    return 1;
}

void solve(int v)
{
    st[v].insert(Mp(sz[v], v));
    for (pii uu : g[v])
    {
        int u = uu.F;
        solve(u);
        ok[v] &= ok[u];
        if (Sz(st[v]) < Sz(st[u]))
            st[v].swap(st[u]);
        for (pii i : st[u])
        {
            ok[v] &= add(v, i);
        }
    }
    return;
}

vector<int> beechtree(int N, int M, vector<int> P, vector<int> C)
{

    /*

    p[i] : parent of node i
    a[i] : the color of the edge between node i and p[i]

    sz[i] : the size of the subtree of node i
    g[i] : vector of <child, color>
    gsz[i] : map of <color, sz of child with that color>
    st[i] : set of <sz, node>

    */

    n = N - 1, m = M;
    for (int i = 1; i <= n; i++)
    {
        p[i] = P[i], a[i] = C[i];
        g[p[i]].pb(Mp(i, a[i]));
    }
    fill(ok, ok + n + 10, 1);
    dfs(0);
    solve(0);

    vector<int> ans(N);
    for (int i = 0; i <= n; i++)
    {
        ans[i] = ok[i];
    }
    return ans;
}
