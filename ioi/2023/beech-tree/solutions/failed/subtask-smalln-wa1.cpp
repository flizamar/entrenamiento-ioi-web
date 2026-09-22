#include "beechtree.h"

#include <set>
#include <algorithm>

using namespace std;
using pii = pair<int, int>;

int n, m;
vector<int> par;
vector<set<int>> ch_colors;
vector<int> t;

bool is_subset(int x, int y)
{
    if (ch_colors[x].size() > ch_colors[y].size())
        swap(x, y);
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
    for (int r = N - 1; r >= 0; --r)
    {
        solve(r);
    }

    return t;
}
