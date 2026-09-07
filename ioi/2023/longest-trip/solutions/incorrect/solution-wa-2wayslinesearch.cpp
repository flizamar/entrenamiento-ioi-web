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
    srand(time(0));
    for (int i = 1; i < N; i++)
        nodes.insert(i);
    int last = 0;
    ans.push_back(0);

    bool flipped = false;
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
                flipped = false;
                break;
            }
        }
        if (!found)
        {
            if (flipped)
                break;
            flipped = true;
            std::reverse(ans.begin(), ans.end());
            auto it = ans.end();
            it--;
            last = (*it);
        }
    }

    return ans;
}
