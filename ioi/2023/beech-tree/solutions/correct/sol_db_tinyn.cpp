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
    int col[N];
    int par[N];
    vector<int> adj[N];
} graph;

void dfs(int u, vector<int> &verts)
{
    verts.push_back(u);
    for (int v : graph.adj[u])
    {
        dfs(v, verts);
    }
}

bool compute(int r)
{
    vector<int> verts;
    dfs(r, verts);

    do
    {
        map<int, int> colCount;
        for (int i = 1; i < verts.size(); ++i)
        {
            int u = verts[i];
            int c = graph.col[u];
            int p = graph.par[u];

            int cnt = colCount[c];
            colCount[c]++;

            if (p != verts[cnt])
            {
                goto end;
            }
        }
        return true;
    end:;
    } while (next_permutation(verts.begin() + 1, verts.end()));

    return false;
}

vector<int> beechtree(int n, int m, vector<int> P, vector<int> C)
{
    for (int i = 1; i < n; ++i)
    {
        int p = P[i];
        int c = C[i];
        graph.par[i] = p;
        graph.col[i] = c;
        graph.adj[p].push_back(i);
    }
    vector<int> ans(n, 0);
    for (int i = 0; i < n; ++i)
    {
        ans[i] = compute(i);
    }
    return ans;
}
