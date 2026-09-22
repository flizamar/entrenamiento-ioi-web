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

bool distinctCols(int u)
{
    for (auto &pr : graph.col[u])
    {
        auto &[col, vec] = pr;
        if (vec.size() > 1)
        {
            return false;
        }
    }
    return true;
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
        graph.cnt[p]++;
    }
    vector<int> ans(n, 0);
    for (int i = 0; i < n; ++i)
    {
        if (graph.col[i].size() == 0)
        {
            ans[i] = 1;
        }
        else if (i != 0)
        {
            ans[i] = distinctCols(i);
        }
        else
        {
            ans[i] = distinctCols(i);
            vector<pii> children;
            for (auto &pr : graph.col[i])
            {
                for (int v : pr.yy)
                {
                    children.push_back({graph.cnt[v], v});
                }
            }
            sort(all(children));
            for (int j = 0; j < children.size() - 1; ++j)
            {
                ans[i] &= check(children[j].yy, children[j + 1].yy);
            }
            ans[i] &= check(children.back().yy, 0);
        }
    }
    return ans;
}
