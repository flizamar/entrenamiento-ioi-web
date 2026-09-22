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
    if (t.size() == 1)
        return vector<int>(s.begin(), s.end());
    if (s.size() == 0)
        return t;

    if (!are_connected({t[0]}, {t.back()}))
    {
        vector<int> ans(s.begin(), s.end());
        ans.insert(ans.end(), t.begin(), t.end());
        return ans;
    }
    int L = t.size();
    for (int i = 0; i + 1 < L; ++i)
    {
        for (int x : s)
        {
            if (are_connected({t[i]}, {x}))
            {
                vector<int> ans(t.begin() + i + 1, t.end());
                ans.insert(ans.end(), t.begin(), t.begin() + i + 1);
                ans.push_back(x);
                s.erase(x);
                ans.insert(ans.end(), s.begin(), s.end());
                return ans;
            }
        }
    }
    if (s.size() > t.size())
        return vector<int>(s.begin(), s.end());

    return t;
}
