#include "beechtree.h"

using namespace std;

vector<int> beechtree(int N, int /*M*/, vector<int> /*P*/, vector<int> C)
{
    vector<int> t(N, 1);
    bool ok = true;
    for (int v = N - 2; v >= 0; --v)
    {
        t[v] = ok;
        if (C[v] != C[v + 1])
            ok = false;
    }
    return t;
}
