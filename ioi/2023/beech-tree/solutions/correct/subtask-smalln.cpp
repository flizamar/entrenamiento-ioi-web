#include "beechtree.h"

#include <array>

using namespace std;
using pii = pair<int, int>;
using arr = array<int, 501>;

int n, m;
vector<int> par, sz;
vector<arr> ch_colors;
vector<vector<bool>> is_subset;
vector<int> t;

bool dfs(int u, int v)
{
    if (sz[u] > sz[v])
        return false;
    for (int c = 1; c <= m; ++c)
    {
        if (ch_colors[u][c])
        {
            if (!ch_colors[v][c])
                return false;
            int chu = ch_colors[u][c], chv = ch_colors[v][c];
            if (!dfs(chu, chv))
                return false;
        }
    }
    return true;
}

void solve(int r)
{
    vector<bool> subtree(n, false);
    subtree[r] = true;
    for (int u = r + 1; u < n; ++u)
    {
        if (subtree[par[u]])
        {
            subtree[u] = true;
        }
    }
    for (int x = r; x + 1 < n; ++x)
    {
        if (!subtree[x])
            continue;
        for (int y = x + 1; y < n; ++y)
        {
            if (!subtree[y])
                continue;
            if (!is_subset[x][y])
            {
                t[r] = 0;
                return;
            }
        }
    }
}

std::vector<int> beechtree(int N, int M, std::vector<int> P, std::vector<int> C)
{
    n = N, m = M, par = P;
    ch_colors.assign(N, arr());
    for (int v = 0; v < N; ++v)
        ch_colors[v].fill(0);
    t.assign(N, 1);
    for (int v = 1; v < N; ++v)
    {
        int u = P[v];
        if (ch_colors[u][C[v]])
        {
            t[u] = 0;
        }
        else
        {
            ch_colors[u][C[v]] = v;
        }
    }
    sz.assign(N, 1);
    for (int v = N - 1; v > 0; --v)
    {
        sz[P[v]] += sz[v];
    }
    is_subset.assign(N, vector<bool>(N));
    for (int x = 0; x + 1 < N; ++x)
    {
        for (int y = x + 1; y < N; ++y)
        {
            is_subset[x][y] = (sz[x] <= sz[y] ? dfs(x, y) : dfs(y, x));
        }
    }
    for (int r = N - 1; r >= 0; --r)
    {
        solve(r);
        if (r > 0 && t[r] == 0)
            t[par[r]] = 0;
    }

    return t;
}
