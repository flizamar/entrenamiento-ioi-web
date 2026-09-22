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
    bool gpar[N];
    map<int, vector<int>> col[N];
} graph;

bool check(int u)
{
    vector<int> nleaf;
    for (auto &pr : graph.col[u])
    {
        if (pr.yy.size() > 1)
        {
            return false;
        }
        for (int v : pr.yy)
        {
            if (!graph.col[v].empty())
            {
                nleaf.push_back(v);
            }
        }
    }
    if (nleaf.size() > 1)
    {
        return false;
    }
    if (nleaf.size() == 0)
    {
        return true;
    }
    int v = nleaf[0];
    if (graph.gpar[v])
    {
        return false;
    }
    bool res = true;
    for (auto &pr : graph.col[v])
    {
        res &= graph.col[u].count(pr.xx);
    }
    return res;
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
        if (P[i] != 0)
        {
            graph.gpar[P[P[i]]] = true;
        }
    }
    vector<int> ans;
    for (int i = 0; i < n; ++i)
    {
        ans.push_back(check(i));
    }
    return ans;
}
