#include "longesttrip.h"

using namespace std;

vector<int> longest_trip(int N, int /*D*/)
{
    vector<int> t1 = {0}, t2 = {1};
    for (int i = 2; i < N; ++i)
    {
        if (t2.empty())
        {
            t2 = {i};
            continue;
        }
        bool add1 = are_connected({t1.back()}, {i});
        bool add2 = are_connected({t2.back()}, {i});
        if (add1 && add2)
        {
            t1.push_back(i);
            while (!t2.empty())
            {
                t1.push_back(t2.back());
                t2.pop_back();
            }
        }
        else if (add1)
        {
            t1.push_back(i);
        }
        else if (add2)
        {
            t2.push_back(i);
        }
        else
        {
            while (!t2.empty())
            {
                t1.push_back(t2.back());
                t2.pop_back();
            }
            t2 = {i};
        }
    }

    return t1.size() > t2.size() ? t1 : t2;
}
