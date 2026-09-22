#include "beechtree.h"

#include <map>
#include <set>

using namespace std;
using pii = pair<int, int>;

int n, m;
vector<int> par, col, sz, ord;
vector<vector<int>> ch;
vector<int> heavy, head;
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

void solve(int u)
{
    if (u == -1 || t[u] == 1)
        return;
    if (u == head[u])
    {
        vector<int> hpath = {u};
        for (int v = heavy[u]; v != -1; v = heavy[v])
        {
            hpath.push_back(v);
        }
        int lo = 0, hi = hpath.size();
        if (chk(u))
        {
            t[u] = 1;
            return;
        }
        while (lo + 1 < hi)
        {
            int mid = (lo + hi) / 2;
            int v = hpath[mid];
            if (chk(v))
            {
                hi = mid;
            }
            else
            {
                lo = mid;
            }
        }
        if (hi < (int)hpath.size())
            t[hpath[hi]] = 1;
        for (int i = 0; i < hi; ++i)
            t[hpath[i]] = 0;
    }
    for (int v : ch[u])
    {
        if (v != heavy[u])
            solve(v);
    }
    solve(heavy[u]);
}

void dfs(int u)
{
    int max_vsz = 0;
    for (int v : ch[u])
    {
        dfs(v);
        if (sz[v] > max_vsz)
            max_vsz = sz[v], heavy[u] = v;
    }
}

void decomp(int v, int h)
{
    head[v] = h;
    if (heavy[v] != -1)
        decomp(heavy[v], h);
    for (int c : ch[v])
    {
        if (c != heavy[v])
            decomp(c, c);
    }
}

void init()
{
    heavy = vector<int>(n, -1);
    head = vector<int>(n);
    dfs(0);
    decomp(0, 0);
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
    init();
    solve(0);
    for (int v = 1; v < N; ++v)
    {
        if (t[par[v]] == 1)
            t[v] = 1;
    }

    return t;
}
