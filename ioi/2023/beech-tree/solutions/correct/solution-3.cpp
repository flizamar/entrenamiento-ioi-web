#include "beechtree.h"

#include <map>
#include <set>

using namespace std;
using pii = pair<int, int>;

int n, m;
vector<int> par, col, sz, ord;
vector<vector<int>> ch;
vector<bool> rem;
vector<int> sz_rem;
set<pair<pii, int>> q;
map<int, int> cnt;
vector<int> t;

bool chk(int u)
{
    ord.clear();
    q.insert({{-sz[u], 0}, u});
    while (!q.empty())
    {
        int x = q.begin()->second;
        q.erase(q.begin());
        ord.push_back(x);
        for (int y : ch[x])
        {
            q.insert({{-sz[y], (int)ord.size()}, y});
        }
    }
    cnt.clear();
    for (int k = 1; k < (int)ord.size(); ++k)
    {
        int v = ord[k];
        int l = (k == 1 ? 0 : cnt[col[v]]);
        if (par[v] != ord[l])
        {
            return false;
        }
        cnt[col[v]]++;
    }
    return true;
}

int dfs(int u, int p = -1)
{
    sz_rem[u] = 1;
    for (int v : ch[u])
    {
        if (v != p && !rem[v])
        {
            sz_rem[u] += dfs(v, u);
        }
    }
    int v = par[u];
    if (v >= 0 && v != p && !rem[v])
    {
        sz_rem[u] += dfs(v, u);
    }
    return sz_rem[u];
}

int get_centroid(int u, int csz, int p = -1)
{
    for (int v : ch[u])
    {
        if (v != p && !rem[v])
        {
            if (sz_rem[v] * 2 > csz)
            {
                return get_centroid(v, csz, u);
            }
        }
    }
    int v = par[u];
    if (v >= 0 && v != p && !rem[v])
    {
        if (sz_rem[v] * 2 > csz)
        {
            return get_centroid(v, csz, u);
        }
    }
    return u;
}

void solve(int r)
{
    int c = get_centroid(r, dfs(r));

    if (t[c] == -1)
    {
        t[c] = chk(c);
        if (t[c] == 0)
            for (int v = par[c]; v >= 0 && t[v] == -1; v = par[v])
            {
                t[v] = 0;
            }
    }

    rem[c] = true;
    if (t[c] != 1)
    {
        for (int v : ch[c])
        {
            if (!rem[v])
            {
                solve(v);
            }
        }
    }
    int v = par[c];
    if (v >= 0 && !rem[v])
        solve(v);
}

std::vector<int> beechtree(int N, int M, std::vector<int> P, std::vector<int> C)
{
    n = N, m = M, par = P, col = C;
    ch.assign(N, {});
    sz.assign(N, 1);
    t.assign(N, -1);
    for (int v = 1; v < N; ++v)
    {
        int u = par[v];
        ch[u].push_back(v);
    }
    for (int v = N - 1; v > 0; --v)
    {
        sz[par[v]] += sz[v];
    }

    rem.assign(N, false), sz_rem.assign(N, 0);
    solve(0);
    for (int v = 1; v < N; ++v)
    {
        if (t[par[v]] == 1)
            t[v] = 1;
    }

    return t;
}
