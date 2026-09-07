#include "longesttrip.h"
#include <deque>

std::vector<int> longest_trip(int N, int D)
{
    std::deque<int> res;
    int nxt;
    if (are_connected({0}, {1}))
    {
        res = {0, 1};
        nxt = 2;
    }
    else
    {
        res = {0, 2, 1};
        nxt = 3;
    }

    while (nxt < N)
    {
        std::vector<int> A = {res.back()}, B = {nxt};
        if (are_connected(A, B))
        {
            res.push_back(nxt);
        }
        else
        {
            res.push_front(nxt);
        }

        nxt++;
    }

    std::vector<int> ans;
    for (auto i : res)
        ans.push_back(i);

    return ans;
}
