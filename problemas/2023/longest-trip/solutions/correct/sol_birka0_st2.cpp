#include "longesttrip.h"

using namespace std;

vector<int> longest_trip(int N, int /*D*/)
{
    vector<int> t = {0};
    int mx = N - 1;
    for (int i = 0; i + 1 <= mx; ++i)
    {
        if (!are_connected({i}, {i + 1}))
        {
            if (i + 1 < mx)
            {
                t.push_back(mx);
                --mx;
            }
            else
            {
                t.push_back(0);
                t.erase(t.begin());
            }
        }
        t.push_back(i + 1);
    }
    return t;
}
