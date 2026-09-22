#include "longesttrip.h"
#include <algorithm>
#include <random>
#include <set>

std::vector<int> longest_trip(int N, int /*D*/)
{
    std::set<int> nodes;
    nodes.clear();
    std::vector<int> ans;
    ans.clear();
    for (int i = 1; i < N; i++)
        nodes.insert(i);
    int last = 0;
    ans.push_back(0);

    while (!nodes.empty())
    {
        bool found = false;
        for (int i : nodes)
        {
            if (are_connected({last}, {i}))
            {
                last = i;
                nodes.erase(i);
                ans.push_back(last);
                found = true;
                break;
            }
        }
        if (!found)
            break;
    }
    return ans;
}
