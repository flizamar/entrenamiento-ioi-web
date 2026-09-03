#include "beechtree.h"

#include <set>
#include <algorithm>

using namespace std;

vector<int> beechtree(int N, int /*M*/, vector<int> P, vector<int> C)
{
    vector<set<int>> ch_colors(N);
    vector<int> t(N, 1);
    for (int v = 1; v < N; ++v)
    {
        int u = P[v], c = C[v];
        if (ch_colors[u].count(c))
        {
            t[u] = false;
            if (u > 0)
                t[0] = false;
        }
        else
            ch_colors[u].insert(c);
    }

    vector<int> ord(N);
    for (int i = 0; i < N; ++i)
        ord[i] = i;
    sort(ord.begin(), ord.end(), [&](int u, int v)
         {
        if (ch_colors[u].size() != ch_colors[v].size())
            return ch_colors[u].size() < ch_colors[v].size();
        return u > v; });
    for (int i = 0; i + 1 < N; ++i)
    {
        int u = ord[i], v = ord[i + 1];
        for (int c : ch_colors[u])
        {
            if (!ch_colors[v].count(c))
                t[0] = false;
        }
    }

    return t;
}
