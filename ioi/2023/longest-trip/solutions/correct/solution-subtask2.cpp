#include "longesttrip.h"
#include <algorithm>
#include <random>
#include <deque>

std::vector<int> longest_trip(int N, int /*D*/)
{
    srand(time(0));
    std::vector<int> ids(N);
    for (int i = 0; i < N; i++)
        ids[i] = i;
    random_shuffle(ids.begin(), ids.end());

    std::deque<int> line;
    line.clear();

    if (!are_connected({ids[0]}, {ids[1]}))
    {
        std::swap(ids[0], ids[2]);
    }
    line.push_back(ids[0]);
    line.push_back(ids[1]);
    for (int i = 2; i < N; i++)
    {
        if (are_connected({ids[i]}, {line.back()}))
        {
            line.push_back(ids[i]);
        }
        else if (are_connected({ids[i]}, {line.front()}))
        {
            line.push_front(ids[i]);
        }
        else
        {
            break;
        }
    }
    std::vector<int> ans;
    ans.clear();
    for (int i : line)
        ans.push_back(i);
    return ans;
}
