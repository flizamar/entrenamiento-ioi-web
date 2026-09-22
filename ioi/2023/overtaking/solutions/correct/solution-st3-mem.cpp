#include "overtaking.h"

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

#define MAXNM 1001000
#define MAXTIME 1000000000000000000

using namespace std;

long long L, N, M, X;
long long a;
int cars[MAXNM];
long long T[MAXNM];
long long W[MAXNM];
long long S[MAXNM];
long long currT[MAXNM];
set<pair<long long, long long>> ans[MAXNM];
vector<pair<int, long long>> mem;

bool ahead(int x, int y)
{
    return currT[x] < currT[y] || (currT[x] == currT[y] && W[x] < W[y]);
}

void init(int LL, int NN, std::vector<long long> TT, std::vector<int> WW, int XX, int MM, std::vector<int> SS)
{
    L = LL;
    N = NN;
    M = MM;
    X = XX;
    for (int i = 0; i < N; i++)
        cars[i] = i;
    int id = 0;
    for (long long i : TT)
        T[id++] = i;
    id = 0;
    for (int i : WW)
        W[id++] = i;
    id = 0;
    for (int i : SS)
        S[id++] = i;
    N++;
}

long long arrival_time(long long Y)
{
    T[N - 1] = Y;
    W[N - 1] = X;
    for (int i = 0; i < N; i++)
    {
        cars[i] = i;
        currT[i] = T[i];
    }
    sort(cars, cars + N, ahead);
    mem.clear();

    bool found = false;
    for (int s = 1; s < M; s++)
    {
        long long d = S[s] - S[s - 1];
        long long maxTime = 0;
        for (int i = 0; i < N; i++)
        {
            int car = cars[i];
            if (car == N - 1 && maxTime >= currT[car] + W[car] * d)
            {
                auto ansIt = ans[s].lower_bound({maxTime, -1});
                if (ansIt != ans[s].end() && (*ansIt).first == maxTime)
                {
                    currT[N - 1] = (*ansIt).second;
                    found = true;
                    break;
                }
                else
                {
                    mem.push_back({s, maxTime});
                }
            }
            maxTime = max(maxTime, currT[car] + W[car] * d);
            currT[car] = maxTime;
        }
        if (found)
            break;
        sort(cars, cars + N, ahead);
    }

    for (auto it : mem)
        ans[it.first].insert({it.second, currT[N - 1]});

    return currT[N - 1];
}