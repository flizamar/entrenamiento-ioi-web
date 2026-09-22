#include "longesttrip.h"

#include <algorithm>
#include <random>

using namespace std;

random_device rd;
mt19937 g(rd());

vector<int> longest_trip(int N, int /*D*/)
{
    vector<int> t;
    for (int i = 0; i < N; ++i)
    {
        t.push_back(i);
    }
    bool ok = false;
    while (!ok)
    {
        shuffle(t.begin(), t.end(), g);
        ok = true;
        for (int i = 0; i + 1 < N; ++i)
        {
            if (!are_connected({t[i]}, {t[i + 1]}))
            {
                ok = false;
                break;
            }
        }
    }

    return t;
}
