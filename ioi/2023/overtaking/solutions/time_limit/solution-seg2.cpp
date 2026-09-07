#include "overtaking.h"

#include <vector>
#include <algorithm>
#include <iostream>

#define MAXNM 1001000
#define MAXTIME 1000000000000000000

using namespace std;

long long a;
int cars[MAXNM];
long long T[MAXNM];
long long W[MAXNM];
long long S[MAXNM];

struct segment
{
    long long a, b, c, mult;
    segment(long long aa, long long bb, long long cc, long long multt)
    {
        a = aa;
        b = bb;
        c = cc;
        mult = multt;
    }
    segment() = default;
};
vector<segment> segments[2];
int ansSize;
segment ans[MAXNM];

bool ahead(int x, int y)
{
    return T[x] < T[y] || (T[x] == T[y] && W[x] < W[y]);
}

void init(int /*L*/, int N, std::vector<long long> TT, std::vector<int> WW, int X, int M, std::vector<int> SS)
{
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

    sort(cars, cars + N, ahead);
    a = 0;
    for (int i = 0; i < N; i++)
    {
        int car = cars[i];
        if (T[car] < a)
            continue;

        if (a < T[car])
        {
            segments[0].push_back(segment(a, T[car] - 1, a, 1));
            a = T[car];
        }
        segments[0].push_back(segment(a, a, a, 1));
        a = T[car] + 1;
    }
    if (a <= MAXTIME)
    {
        segments[0].push_back(segment(a, MAXTIME, a, 1));
    }

    for (int s = 1; s < M; s++)
    {
        long long d = S[s] - S[s - 1];
        int lastCar = 0;
        long long lastTime = 0;
        segments[s % 2].clear();
        for (const segment &seg : segments[(s - 1) % 2])
        {
            while (lastCar < N && (T[cars[lastCar]] < seg.c || (T[cars[lastCar]] == seg.c && W[cars[lastCar]] < X)))
            {
                lastTime = max(lastTime, T[cars[lastCar]] + d * W[cars[lastCar]]);
                T[cars[lastCar]] = lastTime;
                lastCar++;
            }

            long long topTime = lastTime - d * X;
            if (seg.c <= topTime)
            {
                if (seg.mult == 0)
                {
                    segments[s % 2].push_back(segment(seg.a, seg.b, lastTime, 0));
                }
                else
                {
                    long long bb = topTime - (seg.c - seg.a);
                    segments[s % 2].push_back(segment(seg.a, min(bb, seg.b), lastTime, 0));
                    bb++;
                    if (bb <= seg.b)
                        segments[s % 2].push_back(segment(bb, seg.b, seg.c + (bb - seg.a) + d * X, 1));
                }
            }
            else
            {
                segments[s % 2].push_back(segment(seg.a, seg.b, seg.c + d * X, seg.mult));
            }
        }
        while (lastCar < N)
        {
            lastTime = max(lastTime, T[cars[lastCar]] + d * W[cars[lastCar]]);
            T[cars[lastCar]] = lastTime;
            lastCar++;
        }

        sort(cars, cars + N, ahead);
    }

    for (const segment &seg : segments[(M - 1) % 2])
    {
        ans[ansSize++] = seg;
    }
}

long long binSearch(long long Y, int l, int r)
{
    if (l == r)
    {
        return ans[l].c + ans[l].mult * (Y - ans[l].a);
    }

    long long half = (l + r) / 2;
    if (Y <= ans[half].b)
        return binSearch(Y, l, half);
    return binSearch(Y, half + 1, r);
}

long long arrival_time(long long Y)
{
    return binSearch(Y, 0, ansSize - 1);
}