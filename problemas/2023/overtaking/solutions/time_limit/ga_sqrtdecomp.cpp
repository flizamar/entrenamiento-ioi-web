#include "overtaking.h"
#include <algorithm>

using namespace std;
using ll = long long;

const int BLOCK_SIZE = 110;

struct iv
{
    ll start;
    ll end;
    ll dest;
};

vector<iv> merge_intervals(const vector<iv> &iv1, const vector<iv> &iv2)
{
    vector<iv> result;
    int i = 0;
    for (int j = 0; j < iv2.size(); j++)
    {
        while (i < iv1.size() && iv1[i].dest <= iv2[j].start)
        {
            result.push_back(iv1[i]);
            i++;
        }
        ll start = iv2[j].start;
        if (i < iv1.size() && iv1[i].start <= start)
        {
            if (iv1[i].dest > iv2[j].end)
                continue;
            start = iv1[i].start;
        }
        while (i < iv1.size() && iv1[i].dest <= iv2[j].end)
        {
            i++;
        }
        ll end = iv2[j].end;
        if (i < iv1.size() && iv1[i].start <= end)
        {
            end = iv1[i].start;
        }
        result.push_back({start, end, iv2[j].dest});
    }
    while (i < iv1.size())
    {
        result.push_back(iv1[i]);
        i++;
    }
    return result;
}

vector<vector<iv>> intervals;
ll total_time;

void init(int L, int N, std::vector<long long> T, std::vector<int> W, int X, int M, std::vector<int> S)
{
    total_time = L * (ll)X;
    vector<pair<ll, int>> initial(N);
    for (int i = 0; i < N; i++)
    {
        if (W[i] > X)
        {
            initial.emplace_back(T[i], W[i] - X);
        }
    }
    N = initial.size();
    sort(initial.begin(), initial.end());
    vector<vector<ll>> bus_time(M, vector<ll>(N));
    vector<vector<int>> bus_rv(M, vector<int>(N));
    for (int j = 0; j < N; j++)
    {
        bus_time[0][j] = initial[j].first;
        bus_rv[0][j] = initial[j].second;
    }
    for (int i = 1; i < M; i++)
    {
        ll dist = S[i] - S[i - 1];
        ll m = 0;
        for (int j = 0; j < N; j++)
        {
            int rv = bus_rv[i - 1][j];
            ll t = bus_time[i - 1][j] + rv * dist;
            if (t <= m)
            {
                bus_time[i][j] = m;
                int k = j;
                while (k >= 1 && bus_time[i][k - 1] == m && bus_rv[i][k - 1] > rv)
                {
                    bus_rv[i][k] = bus_rv[i][k - 1];
                    k--;
                }
                bus_rv[i][k] = rv;
            }
            else
            {
                bus_time[i][j] = t;
                bus_rv[i][j] = rv;
                m = t;
            }
        }
    }
    intervals.resize(M / BLOCK_SIZE + 1);
    ll current_block = 0;
    vector<iv> intervals1;
    for (int i = M - 1; i >= 1; i--)
    {
        vector<iv> intervals2;
        ll last_end = 0;
        for (int j = 0; j < N; j++)
        {
            if (bus_time[i][j] > last_end)
            {
                if (bus_time[i - 1][j] < last_end)
                {
                    intervals2.back().end = bus_time[i - 1][j];
                }
                intervals2.push_back(
                    {bus_time[i - 1][j], bus_time[i][j], bus_time[i][j]});
                last_end = bus_time[i][j];
            }
        }
        intervals1 = merge_intervals(intervals2, intervals1);
        if (i % BLOCK_SIZE == 1)
        {
            swap(intervals[i / BLOCK_SIZE], intervals1);
        }
    }
}

long long block_step(const vector<iv> &block, ll Y)
{
    int l = -1;
    int u = block.size();
    while (l + 1 < u)
    {
        int m = (l + u) / 2;
        if (block[m].start < Y && Y <= block[m].end)
        {
            return block[m].dest;
        }
        else if (Y <= block[m].start)
        {
            u = m;
        }
        else
        {
            l = m;
        }
    }
    return Y;
}

long long arrival_time(long long Y)
{
    for (int i = 0; i < intervals.size(); i++)
    {
        Y = block_step(intervals[i], Y);
    }
    return Y + total_time;
}
