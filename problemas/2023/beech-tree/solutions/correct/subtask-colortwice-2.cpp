#include "beechtree.h"

#include <set>
#include <algorithm>

using namespace std;

vector<int> t, dep;
vector<vector<int>> ch;
vector<set<int>> ch_colors;

void dfs(int u)
{
    for (int v : ch[u])
    {
        dfs(v);
        dep[u] = max(dep[u], dep[v] + 1);
        if (t[v] == 0)
            t[u] = 0;
    }
    if (dep[u] != 2)
    {
        if (dep[u] > 2)
            t[u] = 0;
        return;
    }
    vector<int> ord = ch[u];
    ord.push_back(u);
    sort(ord.begin(), ord.end(), [&](int x, int y)
         {
        if (ch_colors[x].size() != ch_colors[y].size())
            return ch_colors[x].size() < ch_colors[y].size();
        return x > y; });
    if (ord.back() != u)
        t[u] = 0;
    for (int i = 0; i + 1 < (int)ord.size(); ++i)
    {
        int x = ord[i], y = ord[i + 1];
        for (int c : ch_colors[x])
        {
            if (!ch_colors[y].count(c))
                t[u] = 0;
        }
    }
}

vector<int> beechtree(int N, int /*M*/, vector<int> P, vector<int> C)
{
    t.assign(N, 1);
    ch.assign(N, {});
    ch_colors.resize(N);
    for (int v = 1; v < N; ++v)
    {
        int u = P[v], c = C[v];
        if (ch_colors[u].count(c))
            t[u] = false;
        ch[u].push_back(v);
        ch_colors[u].insert(c);
    }
    dep.assign(N, 0);
    dfs(0);

    return t;
}
