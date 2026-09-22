#include "longesttrip.h"
#include <algorithm>
#include <random>

std::vector<int> longest_trip(int N, int /*D*/)
{
    srand(time(0));
    std::vector<int> vec(N);
    for (int i = 0; i < N; i++)
        vec[i] = i;
    random_shuffle(vec.begin(), vec.end());
    return vec;
}
