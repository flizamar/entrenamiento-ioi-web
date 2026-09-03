#include "longesttrip.h"

#include <set>
#include <algorithm>

using namespace std;

vector<int> longest_trip(int N, int /*D*/)
{
    vector<int> t = {0};
    set<int> s;
    for (int i = 1; i < N; ++i)
    {
        s.insert(i);
    }
    for (int l = 1; l < N; ++l)
    {
        bool succ = false;
        for (int x : s)
        {
            if (are_connected({t.back()}, {x}))
            {
                t.push_back(x);
                s.erase(x);
                succ = true;
                break;
            }
        }
        if (!succ)
        {
            break;
        }
    }
    if (s.size() > t.size())
    {
        return vector<int>(s.begin(), s.end());
    }

    return t;
}
