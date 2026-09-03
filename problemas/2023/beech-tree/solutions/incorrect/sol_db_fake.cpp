#include "beechtree.h"
#include <bits/stdc++.h>

#define all(x) (x).begin(), (x).end()
#define xx first
#define yy second

using namespace std;
typedef pair<int, int> pii;
const int N = 2e5 + 1;

struct Graph
{
    int cnt[N];
    bool con[N];
    map<int, vector<int>> col[N];
} graph;

bool check(int v, int u)
{
    for (auto &pr : graph.col[v])
    {
        auto &[col, vec] = pr;
        auto it = graph.col[u].find(col);
        if (it == graph.col[u].end() || graph.cnt[vec[0]] > graph.cnt[it->yy[0]])
        {
            return false;
        }
    }
    return true;
}

int dfs(int u)
{
    graph.cnt[u] = 1;
    graph.con[u] = true;

    vector<pii> srt;
    for (auto &pr : graph.col[u])
    {
        auto &[col, vec] = pr;
        graph.con[u] &= vec.size() == 1;

        for (int v : vec)
        {
            int cnv = dfs(v);
            if (cnv == -1)
            {
                graph.con[u] = false;
            }
            else
            {
                graph.cnt[u] += cnv;
            }
            srt.push_back({cnv, v});
        }
    }
    srt.push_back({graph.cnt[u], u});
    sort(all(srt));

    for (int i = 0; i < srt.size() - 1; ++i)
    {
        graph.con[u] = graph.con[u] & check(srt[i].yy, srt[i + 1].yy);
    }
    return graph.con[u] ? graph.cnt[u] : -1;
}

vector<int> beechtree(int n, int m, vector<int> P, vector<int> C)
{
    for (int i = 1; i < n; ++i)
    {
        int p = P[i];
        int c = C[i];
        if (!graph.col[p].count(c))
        {
            graph.col[p][c] = vector<int>();
        }
        graph.col[p][c].push_back(i);
    }
    dfs(0);

    vector<int> ans;
    for (int i = 0; i < n; ++i)
        ans.push_back(graph.con[i]);
    return ans;
}
