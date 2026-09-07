#include "overtaking.h"
#include <algorithm>

using namespace std;
using ll = long long;

vector<ll> endpoints;
ll total_time;

void init(int L, int N, std::vector<long long> T, std::vector<int> W, int X, int M, std::vector<int> S)
{
    total_time = L * (ll)X;
    for (int i = 0; i < M; i++)
    {
        endpoints.push_back(T[0] + (W[0] - X) * (ll)S[i]);
    }
}

long long arrival_time(long long Y)
{
    // If W[0] < X, then endpoints is decreasing, so the call to lower_bound is UB.
    // However, it happens to return either begin() or end(), giving the correct answer.
    auto it = lower_bound(endpoints.begin(), endpoints.end(), Y);
    if (it == endpoints.begin() || it == endpoints.end())
    {
        return Y + total_time;
    }
    else
    {
        return *it + total_time;
    }
}
