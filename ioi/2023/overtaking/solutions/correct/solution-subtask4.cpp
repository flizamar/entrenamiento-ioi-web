#include "overtaking.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <set>

#define MAXNM 1001000
#define MAXTIME 1000000000000000000

using namespace std;

long long L, N, M, X, Y, id, station;
long long a;
long long T[MAXNM];
long long W[MAXNM];
long long S[MAXNM];
vector<long long> currT[MAXNM];
vector<int> cars[MAXNM];
set<pair<int, int>> overtake[MAXNM];

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
    {
        S[id++] = i;
    }

    id = 0;
    for (int i = 0; i < N; i++)
    {
        if (W[i] > X)
        {
            W[id] = W[i];
            T[id] = T[i];
            id++;
        }
    }
    N = id;

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
    std::sort(cars[0].begin(), cars[0].end(), ahead);

    for (int s = 1; s < M; s++)
    {
        long long d = S[s] - S[s - 1];
        long long maxTime = 0, maxCar = -1;
        for (int i = 0; i < N; i++)
        {
            int car = cars[s - 1][i];
            if (maxTime < currT[s - 1][car] + W[car] * d)
            {
                maxTime = currT[s - 1][car] + W[car] * d;
                maxCar = car;
            }
            else
            {
                overtake[car].insert({s, maxCar});
            }
            currT[s][car] = maxTime;
        }
        id = s;
        std::sort(cars[s].begin(), cars[s].end(), ahead);
    }

    for (int i = 0; i < N; i++)
        overtake[i].insert({M, -1});
}

int vertBin(int s, int x, int y)
{
    if (Y <= currT[s][cars[s][0]])
        return -1;

    if (x == y)
        return cars[s][x];

    int half = (x + y + 1) / 2;
    if (currT[s][cars[s][half]] < Y)
        return vertBin(s, half, y);
    return vertBin(s, x, half - 1);
}

int horBin(int carId, int x, int y)
{
    if (x == y)
        return x;

    int half = (x + y) / 2;
    if (Y + X * (S[half] - S[station]) > currT[half][carId])
        return horBin(carId, half + 1, y);
    return horBin(carId, x, half);
}

long long arrival_time(long long YY)
{
    Y = YY;

    if (N == 0)
        return Y + L * X;

    station = 0;
    int below = vertBin(0, 0, N - 1);
    while (station != M - 1)
    {
        if (below == -1)
            return Y + X * (L - S[station]);

        auto it = overtake[below].lower_bound({station + 1, -1});
        int pos = (*it).first;

        int st = pos - 1;
        if (Y + (S[st] - S[station]) * X > currT[st][below])
        {
            Y += (S[st] - S[station]) * X;
            below = (*it).second;
            station = st;
        }
        else
        {
            int ost = horBin(below, station, st);
            Y = currT[ost][below];
            station = ost;
            below = vertBin(station, 0, N - 1);
        }
    }

    return Y;
}