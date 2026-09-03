#include "beechtree.h"

#include <algorithm>

using namespace std;

int n;
vector<int> par, col;

bool check_subtree(vector<int> &subtree)
{
    if (subtree.size() == 2)
    {
        return true;
    }
    do
    {
        bool ok = true;
        for (int k = 1; k < (int)subtree.size(); ++k)
        {
            int v = subtree[k], l = 0;
            for (int i = 1; i < k; ++i)
            {
                l += (col[v] == col[subtree[i]]);
            }
            if (par[v] != subtree[l])
            {
                ok = false;
            }
        }
        if (ok)
        {
            return true;
        }
    } while (next_permutation(subtree.begin() + 1, subtree.end()));

    return false;
}

std::vector<int> beechtree(int N, int /*M*/, std::vector<int> P, std::vector<int> C)
{
    n = N, par = P, col = C;
    vector<int> t(n);
    for (int r = 0; r < n; ++r)
    {
        vector<int> subtree = {r};
        for (int v = r + 1; v < n; ++v)
        {
            if (find(subtree.begin(), subtree.end(), par[v]) != subtree.end())
            {
                subtree.push_back(v);
            }
        }
        t[r] = check_subtree(subtree);
    }

    return t;
}
