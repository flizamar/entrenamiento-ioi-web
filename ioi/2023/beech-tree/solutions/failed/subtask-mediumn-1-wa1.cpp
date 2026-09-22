#include "beechtree.h"

#include <set>
#include <algorithm>

using namespace std;
using pii = pair<int, int>;

int n, m;
vector<int> par;
vector<set<int>> ch_colors;
vector<vector<int>> ch, ord;
vector<int> t;

bool is_subset(int x, int y)
{
    if (ch_colors[x].size() > ch_colors[y].size())
        swap(x, y);
    for (int c : ch_colors[x])
    {
        if (!ch_colors[y].count(c))
            return false;
    }
    return true;
}

void dfs(int u)
{
    for (int v : ch[u])
    {
        dfs(v);
        if (t[v] == 0)
        {
            t[u] = 0;
        }
    }
    if (t[u] == 0)
        return;
    ord[u] = {u};
    for (int v : ch[u])
        ord[u].insert(ord[u].end(), ord[v].begin(), ord[v].end());
    sort(ord[u].begin(), ord[u].end(), [&](int x, int y)
         {
        if (ch_colors[x].size() != ch_colors[y].size())
            return ch_colors[x].size() < ch_colors[y].size();
        return x > y; });
    int k = ord[u].size();
    for (int i = 0; i + 1 < k; ++i)
    {
        int x = ord[u][i], y = ord[u][i + 1];
        if (!is_subset(x, y))
        {
            t[u] = 0;
            return;
        }
    }
}

std::vector<int> beechtree(int N, int M, std::vector<int> P, std::vector<int> C)
{
    n = N, m = M, par = P;
    ord.assign(N, {});
    ch.assign(N, {});
    ch_colors.assign(N, {});
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
