// Incorrectly assumes that if the reserve bus does not meet any other bus,
// then it is either the first one or the last one

#include "overtaking.h"
#include <algorithm>
#include <map>

using namespace std;
using ll = long long;

map<ll, ll> endpoints;
ll total_time, last_bus;

void init(int L, int N, std::vector<long long> T, std::vector<int> W, int X, int M, std::vector<int> S)
{
    total_time = L * (ll)X;
    vector<pair<ll, int>> initial;
    for (int i = 0; i < N; i++)
    {
        initial.emplace_back(T[i], W[i]);
    }
    sort(initial.begin(), initial.end());
    for (int i = 0; i < N; i++)
    {
        T[i] = initial[i].first;
        W[i] = initial[i].second;
    }
    ll m = 0;
    for (int i = 0; i < N; i++)
    {
        if (W[i] > X)
        {
            m = max(m, T[i] + W[i] * (ll)L);
            endpoints[T[i]] = m - X * (ll)L;
        }
    }
    last_bus = m - X * (ll)L;
}

long long arrival_time(long long Y)
{
    auto it = endpoints.lower_bound(Y);
    if (Y >= last_bus || it == endpoints.begin())
    {
        return Y + total_time;
    }
    else
    {
        return prev(it)->second + total_time;
    }
}