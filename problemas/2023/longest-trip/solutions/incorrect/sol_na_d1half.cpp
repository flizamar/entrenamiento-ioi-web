#include "longesttrip.h"
#include <deque>

std::vector<int> longest_trip(int N, int D)
{
    std::deque<int> A = {0}, B;
    for (int i = 1; i < N; ++i)
    {
        if (are_connected({A.back()}, {i}))
        {
            A.push_back(i);
        }
        else if (B.empty() || are_connected({B.back()}, {i}))
        {
            B.push_back(i);
        }
        else
        {
            while (!B.empty())
            {
                A.push_back(B.back());
                B.pop_back();
            }
        }
    }

    if (A.size() > B.size())
        return std::vector<int>(A.begin(), A.end());
    return std::vector<int>(B.begin(), B.end());
}
