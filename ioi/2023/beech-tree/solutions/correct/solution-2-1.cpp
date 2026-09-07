#include "beechtree.h"

#include <queue>
#include <map>
#include <set>

using namespace std;
using pii = pair<int, int>;

int n, m;
vector<int> par, col, sz, ord;
vector<vector<int>> ch;
vector<int> heavy, head;
priority_queue<pair<pii, int>> q;
map<int, int> cnt;
vector<int> t;

bool chk(int u)
{
    ord.clear();
    q.push({{sz[u], n}, u});
    while (!q.empty())
    {
        int x = q.top().second;
        q.pop();
        ord.push_back(x);
        for (int y : ch[x])
        {
            q.push({{sz[y], n - (int)ord.size()}, y});
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
        while (lo < hi && t[hpath[lo]] == 0)
            ++lo;
        if (lo < hi && chk(hpath[lo]))
        {
            hi = lo;
            --lo;
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
    if (t[u] == 1)
        return;
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
    vector<set<int>> ch_colors(N);
    sz.assign(N, 1);
    t.assign(N, -1);
    for (int v = 1; v < N; ++v)
    {
        int u = par[v];
        if (ch_colors[u].count(C[v]))
        {
            t[u] = 0;
        }
        ch[u].push_back(v);
        ch_colors[u].insert(C[v]);
    }
    for (int v = N - 1; v > 0; --v)
    {
        sz[par[v]] += sz[v];
        if (t[v] == 0)
            t[par[v]] = 0;
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
