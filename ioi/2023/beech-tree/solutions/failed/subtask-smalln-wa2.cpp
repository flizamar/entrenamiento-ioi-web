#include "beechtree.h"

#include <set>
#include <algorithm>

using namespace std;
using pii = pair<int, int>;

int n, m;
vector<int> par, sz;
vector<set<int>> ch_colors;
vector<int> t;

bool is_subset(int x, int y)
{
    if (sz[x] > sz[y])
        swap(x, y);
    if (sz[x] == sz[y] && ch_colors[x].size() != ch_colors[y].size())
        return false;
    if (ch_colors[x].size() > ch_colors[y].size())
        return false;
    for (int c : ch_colors[x])
    {
        if (!ch_colors[y].count(c))
            return false;
    }
    return true;
}

void solve(int r)
{
    vector<int> subtree = {r};
    for (int u = r + 1; u < n; ++u)
    {
        if (ch_colors[u].size() == 0)
            continue;
        if (binary_search(subtree.begin(), subtree.end(), par[u]))
        {
            if (t[u] == 0)
            {
                t[r] = 0;
                return;
            }
            subtree.push_back(u);
        }
    }
    int k = subtree.size();
    for (int i = 0; i + 1 < k; ++i)
    {
        for (int j = i + 1; j < k; ++j)
        {
            int x = subtree[i], y = subtree[j];
            if (!is_subset(x, y))
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
    ch_colors.assign(N, {});
    t.assign(N, 1);
    for (int v = 1; v < N; ++v)
    {
        int u = P[v];
        if (ch_colors[u].count(C[v]))
        {
            t[u] = 0;
        }
        ch_colors[u].insert(C[v]);
    }
    for (int v = 1; v < N; ++v)
    {
        if (ch_colors[v].size() > ch_colors[P[v]].size())
            t[P[v]] = 0;
    }
    sz.assign(N, 1);
    for (int r = N - 1; r >= 0; --r)
    {
        if (r > 0)
            sz[P[r]] += sz[r];
        solve(r);
    }

    return t;
}
