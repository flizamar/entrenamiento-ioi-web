#include "beechtree.h"

#include <set>

using namespace std;

vector<int> t, dep;
vector<vector<int>> ch;
vector<set<int>> ch_colors;

void dfs(int u)
{
    int cnt1 = 0, v1 = 0;
    for (int v : ch[u])
    {
        dfs(v);
        dep[u] = max(dep[u], dep[v] + 1);
        if (dep[v] == 1)
        {
            ++cnt1;
            v1 = v;
        }
    }
    if (dep[u] < 2)
        return;
    if (dep[u] > 2 || (dep[u] == 2 && cnt1 > 1))
    {
        t[u] = 0;
        return;
    }
    for (int c : ch_colors[v1])
    {
        if (!ch_colors[u].count(c))
            t[u] = 0;
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
