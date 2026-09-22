#include "beechtree.h"

#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;

#define F first
#define S second

const int xn = 2e5 + 10;

int n, m, par[xn], a[xn], sz[xn];
pii dsu[xn];
vector<int> adj[xn];
set<pii> st[xn], chil[xn];
bool ans[xn], ns[xn];
map<int, int> mp[xn];

bool is_ok(int v, int u)
{
    for (int x : adj[v])
        if (mp[u][a[x]] < sz[x])
            return false;
    return true;
}
int get_root(int v)
{
    if (v == dsu[v].F)
        return v;
    return dsu[v].F = get_root(dsu[v].F);
}
void merge(int v, int u)
{
    v = get_root(v);
    u = get_root(u);
    if (dsu[v].S < dsu[u].S)
        swap(v, u);
    dsu[u].F = v;
    dsu[v].S += dsu[u].S;
    if (!ns[v] || !ns[u])
    {
        ns[v] = false;
        return;
    }
    for (pii x : st[u])
    {
        st[v].insert(x);
        auto it = st[v].find(x);
        it++;
        if (it != st[v].end())
        {
            int next_ver = it->S;
            if (!is_ok(x.S, next_ver))
            {
                ns[v] = false;
                break;
            }
        }
        it--;
        if (it != st[v].begin())
        {
            it--;
            int prev_ver = it->S;
            if (!is_ok(prev_ver, x.S))
            {
                ns[v] = false;
                break;
            }
        }
        st[v].insert(x);
    }
}
void DFS(int v)
{
    sz[v] = 1;
    for (int u : adj[v])
    {
        DFS(u);
        sz[v] += sz[u];
        mp[v][a[u]] = sz[u];
    }
    st[v].insert({sz[v], v});
    dsu[v] = {v, 1};
    ns[v] = true;
    for (int u : adj[v])
        merge(v, u);
    ans[v] = ns[get_root(v)];
}

vector<int> beechtree(int N, int M, vector<int> P, vector<int> C)
{
    n = N - 1, m = M;
    for (int i = 1; i <= n; ++i)
    {
        par[i] = P[i], a[i] = C[i];
        adj[par[i]].push_back(i);
    }
    DFS(0);
    vector<int> ret(N);
    for (int i = 0; i <= n; ++i)
        ret[i] = ans[i];
    return ret;
}
