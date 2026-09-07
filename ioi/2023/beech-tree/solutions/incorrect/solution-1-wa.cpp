#include "beechtree.h"

#include <map>
#include <set>
#include <algorithm>

using namespace std;
using pii = pair<int, int>;

int n;
vector<int> par, col, sz;
vector<map<int, int>> ch;
vector<int> t;
vector<set<pii>> ord;

bool comp(int x, int y)
{
    for (auto [c, sx] : ch[x])
    {
        if (!ch[y].count(c))
            return false;
        int sy = ch[y][c];
        if (sz[sx] > sz[sy])
            return false;
    }
    return true;
}

bool merge(int u, int v)
{
    if (ord[u].size() < ord[v].size())
    {
        swap(ord[u], ord[v]);
    }
    for (auto [s, x] : ord[v])
    {
        auto [it, b] = ord[u].insert({s, x});
        if (it != ord[u].begin() && !comp(prev(it)->second, it->second))
            return false;
        if (next(it) != ord[u].end() && !comp(it->second, next(it)->second))
            return false;
    }
    return true;
}

void dfs(int u)
{
    ord[u].insert({sz[u], u});
    for (auto [c, v] : ch[u])
    {
        dfs(v);
        if (t[v] == 0 || !merge(u, v))
        {
            t[u] = 0;
        }
    }
}

std::vector<int> beechtree(int N, int /*M*/, std::vector<int> P, std::vector<int> C)
{
    n = N, par = P, col = C;
    sz.assign(n, 1);
    ch.resize(n), ord.resize(n);
    t.assign(n, 1);
    for (int v = n - 1; v > 0; --v)
    {
        int u = par[v];
        sz[u] += sz[v];
        if (ch[u].count(col[v]))
        {
            t[u] = 0;
        }
        ch[u][col[v]] = v;
    }
    dfs(0);

    return t;
}
