#include "beechtree.h"

#include <set>
#include <map>

using namespace std;
using pii = pair<int, int>;

int n, m;
vector<int> par, col, sz, ord;
vector<vector<int>> ch;
vector<set<int>> ch_colors;
set<pair<pii, int>> q;
map<int, int> cnt;
vector<int> t;

void dfs(int u)
{
    for (int v : ch[u])
    {
        dfs(v);
        sz[u] += sz[v];
        if (t[v] == 0)
        {
            t[u] = 0;
        }
    }
    if (t[u] == 0)
        return;
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
            t[u] = 0;
            return;
        }
        cnt[col[v]]++;
    }
}

std::vector<int> beechtree(int N, int M, std::vector<int> P, std::vector<int> C)
{
    n = N, m = M, par = P, col = C;
    ch.assign(N, {});
    ch_colors.assign(N, {});
    sz.assign(N, 1);
    t.assign(N, 1);
    for (int v = 1; v < N; ++v)
    {
        int u = P[v];
        if (ch_colors[u].count(C[v]))
        {
            t[u] = 0;
        }
        ch[u].push_back(v);
        ch_colors[u].insert(C[v]);
    }
    for (int v = 1; v < N; ++v)
    {
        if (ch_colors[v].size() > ch_colors[P[v]].size())
            t[P[v]] = 0;
    }
    dfs(0);

    return t;
}
