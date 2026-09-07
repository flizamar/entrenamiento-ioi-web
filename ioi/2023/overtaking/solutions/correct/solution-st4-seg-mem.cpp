#include "overtaking.h"

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

#define MAXNM 1001000
#define MAXTIME 1000000000000000000

using namespace std;

long long L, N, M, X, Y, id;
long long a;
long long T[MAXNM];
long long W[MAXNM];
long long S[MAXNM];
vector<long long> currT[MAXNM];
vector<int> cars[MAXNM];
set<pair<long long, long long>> ans[MAXNM];
vector<pair<int, long long>> mem;

bool ahead(int x, int y)
{
    return currT[id][x] < currT[id][y] || (currT[id][x] == currT[id][y] && W[x] < W[y]);
}

void init(int LL, int NN, std::vector<long long> TT, std::vector<int> WW, int XX, int MM, std::vector<int> SS)
{
    L = LL;
    N = NN;
    M = MM;
    X = XX;
    id = 0;
    for (long long i : TT)
        T[id++] = i;
    id = 0;
    for (int i : WW)
        W[id++] = i;
    id = 0;
    for (int i : SS)
        S[id++] = i;

    for (int s = 0; s < M; s++)
    {
        currT[s] = vector<long long>(N);
        cars[s] = vector<int>(N);
        for (int i = 0; i < N; i++)
            cars[s][i] = i;
    }

    for (int i = 0; i < N; i++)
    {
        currT[0][i] = T[i];
    }
    id = 0;
    sort(cars[0].begin(), cars[0].end(), ahead);

    for (int s = 1; s < M; s++)
    {
        long long d = S[s] - S[s - 1];
        long long maxTime = 0;
        for (int i = 0; i < N; i++)
        {
            int car = cars[s - 1][i];
            maxTime = max(maxTime, currT[s - 1][car] + W[car] * d);
            currT[s][car] = maxTime;
        }
        id = s;
        sort(cars[s].begin(), cars[s].end(), ahead);
    }
}

int binSearch(long long l, long long r)
{
    if (currT[id][cars[id][0]] > Y || (currT[id][cars[id][0]] == Y && W[cars[id][0]] >= X))
    {
        return -1;
    }

    if (l == r)
        return cars[id][l];
    long long half = (l + r + 1) / 2;
    if (currT[id][cars[id][half]] < Y || (currT[id][cars[id][half]] == Y && W[cars[id][half]] < X))
        return binSearch(half, r);
    return binSearch(l, half - 1);
}

long long arrival_time(long long YY)
{
    Y = YY;

    mem.clear();
    for (int s = 1; s < M; s++)
    {
        long long d = S[s] - S[s - 1];
        id = s - 1;
        int car = binSearch(0, N - 1);
        Y += X * d;
        if (car != -1)
        {
            if (Y <= currT[s][car])
            {
                auto ansIt = ans[s].lower_bound({currT[s][car], -1});
                if (ansIt != ans[s].end() && (*ansIt).first == currT[s][car])
                {
                    Y = (*ansIt).second;
                    break;
                }
                else
                {
                    mem.push_back({s, currT[s][car]});
                }
            }
            Y = max(Y, currT[s][car]);
        }
    }

    for (auto it : mem)
        ans[it.first].insert({it.second, Y});

    return Y;
}