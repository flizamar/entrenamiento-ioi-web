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
    vector<int> adj[N][2];
    vector<pii> srt[N];
} graph;

bool check(int v, int u)
{
    for (int c = 0; c < 2; ++c)
    {
        if (graph.adj[v][c].empty())
        {
            continue;
        }
        if (graph.adj[u][c].empty())
        {
            return false;
        }
        if (graph.cnt[graph.adj[v][c][0]] > graph.cnt[graph.adj[u][c][0]])
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

    for (int c = 0; c < 2; ++c)
    {
        auto &vec = graph.adj[u][c];
        graph.con[u] &= vec.size() <= 1;

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
            for (pii p : graph.srt[v])
            {
                graph.srt[u].push_back(p);
            }
        }
    }
    graph.srt[u].push_back({graph.cnt[u], u});
    sort(all(graph.srt[u]));

    for (int i = 0; i < graph.srt[u].size() - 1; ++i)
    {
        graph.con[u] = graph.con[u] & check(graph.srt[u][i].yy, graph.srt[u][i + 1].yy);
    }
    return graph.con[u] ? graph.cnt[u] : -1;
}

vector<int> beechtree(int n, int m, vector<int> P, vector<int> C)
{
    for (int i = 1; i < n; ++i)
    {
        int p = P[i];
        int c = C[i];
        graph.adj[p][c - 1].push_back(i);
    }
    dfs(0);

    vector<int> ans;
    for (int i = 0; i < n; ++i)
        ans.push_back(graph.con[i]);
    return ans;
}