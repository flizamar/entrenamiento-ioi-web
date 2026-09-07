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

    return t;
}
