#include "longesttrip.h"

std::vector<int> longest_trip(int N, int D)
{
    std::vector<int> res;
    for (int i = 0; i < N; ++i)
    {
        res.push_back(i);
    }
    return res;
}
