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
    set<pii> srt[N];
    map<int, vector<int>> col[N];
} graph;

bool check(int v, int u)
{
    for (auto &pr : graph.col[v])
    {
        auto &[col, vec] = pr;
        auto it = graph.col[u].find(col);
        if (it == graph.col[u].end())
        {
            return false;
        }
        if (graph.cnt[vec[0]] > graph.cnt[it->yy[0]])
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
            if (graph.con[u])
            {
                if (graph.srt[v].size() > graph.srt[u].size())
                {
                    graph.srt[u].swap(graph.srt[v]);
                }
                for (pii p : graph.srt[v])
                {
                    auto [it, st] = graph.srt[u].insert(p);
                    if (it != graph.srt[u].begin())
                    {
                        graph.con[u] &= check(prev(it)->yy, it->yy);
                    }
                    if (next(it) != graph.srt[u].end())
                    {
                        graph.con[u] &= check(it->yy, next(it)->yy);
                    }
                }
            }
        }
    }
    graph.srt[u].insert({graph.cnt[u], u});
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
