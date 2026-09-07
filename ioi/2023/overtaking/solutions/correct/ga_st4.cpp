#include "overtaking.h"
#include <algorithm>

using namespace std;
using ll = long long;

ll total_time;
vector<vector<ll>> bus_time;
std::vector<int> S;
ll X;

void init(int L, int N, std::vector<long long> T, std::vector<int> W, int X, int M, std::vector<int> S)
{
    total_time = L * (ll)X;
    ::S = S;
    ::X = X;
    vector<pair<ll, int>> initial;
    for (int i = 0; i < N; i++)
    {
        if (W[i] >= X)
        {
            initial.emplace_back(T[i], W[i]);
        }
    }
    N = initial.size();
    sort(initial.begin(), initial.end());
    bus_time.resize(M, vector<ll>(N));
    vector<int> bus_rv(N);
    for (int j = 0; j < N; j++)
    {
        bus_time[0][j] = initial[j].first;
        bus_rv[j] = initial[j].second;
    }
    for (int i = 1; i < M; i++)
    {
        ll dist = S[i] - S[i - 1];
        ll m = 0;
        for (int j = 0; j < N; j++)
        {
            int rv = bus_rv[j];
            ll t = bus_time[i - 1][j] + rv * dist;
            if (t <= m)
            {
                bus_time[i][j] = m;
                int k = j;
                while (k >= 1 && bus_time[i][k - 1] == m && bus_rv[k - 1] > rv)
                {
                    bus_rv[k] = bus_rv[k - 1];
                    k--;
                }
                bus_rv[k] = rv;
            }
            else
            {
                bus_time[i][j] = t;
                bus_rv[j] = rv;
                m = t;
            }
        }
    }
}

long long arrival_time(long long Y)
{
    for (int i = 0; i < bus_time.size() - 1; i++)
    {
        auto j = lower_bound(bus_time[i].begin(), bus_time[i].end(), Y) - bus_time[i].begin();
        Y += (S[i + 1] - S[i]) * (ll)X;
        if (j != 0)
        {
            Y = max(Y, bus_time[i + 1][j - 1]);
        }
    }
    return Y;
}