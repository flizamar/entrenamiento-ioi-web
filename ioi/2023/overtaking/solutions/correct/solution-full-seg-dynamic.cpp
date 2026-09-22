#include "overtaking.h"

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

#define MAXNM 1001000
#define MAXTIME 1000000000000000000

using namespace std;

long long a, aa, bb, L, X;
int cars[MAXNM];
long long T[MAXNM];
long long W[MAXNM];
long long S[MAXNM];

struct segment
{
    long long a, b, mult;
    segment(long long a2, long long b2, long long multt)
    {
        a = a2;
        b = b2;
        mult = multt;
    }
    segment() = default;
};
inline bool operator<(const segment &xx, const segment &yy)
{
    return xx.a < yy.a;
}
segment base = segment(-1, -1, -1);
set<pair<long long, segment>> segments;
int ansSize;
pair<long long, segment> ans[5 * MAXNM];

bool ahead(int x, int y)
{
    return T[x] < T[y] || (T[x] == T[y] && W[x] < W[y]);
}

void init(int LL, int N, std::vector<long long> TT, std::vector<int> WW, int XX, int M, std::vector<int> SS)
{
    L = LL;
    X = XX;
    int id = 0;
    for (long long i : TT)
        T[id++] = i;
    id = 0;
    for (int i : WW)
        W[id++] = i;

    int slowerCount = 0;
    for (int i = 0; i < N; i++)
    {
        if (W[i] > X)
        {
            W[slowerCount] = W[i];
            T[slowerCount] = T[i];
            cars[slowerCount] = slowerCount;
            slowerCount++;
        }
    }
    N = slowerCount;
    cars[N] = N;
    T[N] = 5 * MAXTIME;

    id = 0;
    for (int i : SS)
    {
        if (id != 0)
        {
            S[id - 1] = i;
        }
        id++;
    }
    M--;

    sort(cars, cars + N, ahead);
    a = -1;
    for (int i = 0; i < N; i++)
    {
        int car = cars[i];
        if (T[car] == a)
            continue;

        segments.insert({a + 1 + L * X, segment(a + 1, T[car], 1)});
        a = T[car];
    }
    if (a + 1 <= MAXTIME)
    {
        segments.insert({a + 1 + L * X, segment(a + 1, MAXTIME, 1)});
    }

    for (int s = 0; s < M; s++)
    {
        long long d = (s == 0 ? S[0] : S[s] - S[s - 1]);
        long long lastTime = 0, currentTime = -1;

        vector<pair<long long, segment>> vec;
        vec.clear();

        for (int lastCar = 0; lastCar <= N; lastCar++)
        {
            if (currentTime < T[cars[lastCar]])
            {
                aa = currentTime + 1;
                bb = T[cars[lastCar]];
                long long faa = aa + (L - S[s] + d) * X;
                long long fbb = bb + (L - S[s] + d) * X;
                long long fLast = lastTime + (L - S[s]) * X;

                long long upper = min(fbb, fLast);

                auto it = segments.lower_bound({upper + 1, base});
                while (it != segments.begin())
                {
                    it--;
                    segment seg = (*it).second;
                    long long f = (*it).first;
                    long long f2 = f + seg.b - seg.a;
                    if (seg.mult == 0)
                        f2 = f;
                    if (fbb < f2)
                    {
                        cout << "WARNING " << s << ' ' << lastCar << ' ' << seg.a << ' ' << seg.b << " - " << aa << ' ' << bb << " = " << f << ' ' << f2 << " - " << faa << ' ' << fbb << endl;
                    }
                    if (f < faa && faa <= f2)
                    {
                        cout << "WARNING2 " << S[s - 2] << ' ' << S[s - 1] << " --- " << ' ' << lastCar << ' ' << seg.a << ' ' << seg.b << " - " << aa << ' ' << bb << " = " << f << ' ' << f2 << " - " << faa << ' ' << fbb << endl;
                    }

                    if (f2 < faa)
                        break;

                    if (seg.mult == 0)
                    {
                        vec.push_back({fLast, seg});
                    }
                    else
                    {
                        vec.push_back({fLast, segment(seg.a, min(seg.b, seg.a + (fLast - f)), 0)});
                        if (seg.a + (fLast - f) < seg.b)
                        {
                            vec.push_back({fLast + 1, segment(seg.a + (fLast - f) + 1, seg.b, 1)});
                        }
                    }

                    segments.erase(it);
                    it = segments.lower_bound({upper + 1, base});
                }
            }

            if (lastCar != N)
            {
                lastTime = max(lastTime, T[cars[lastCar]] + d * W[cars[lastCar]]);
                currentTime = T[cars[lastCar]];
                T[cars[lastCar]] = lastTime;
            }
        }

        for (auto seg : vec)
            segments.insert(seg);

        sort(cars, cars + N, ahead);
    }

    for (auto seg : segments)
    {
        ans[ansSize++] = seg;
    }
}

long long binSearch(long long Y, long long l, long long r)
{
    if (l == r)
    {
        return ans[l].first + ans[l].second.mult * (Y - ans[l].second.a);
    }

    long long half = (l + r) / 2;
    if (Y <= ans[half].second.b)
        return binSearch(Y, l, half);
    return binSearch(Y, half + 1, r);
}

long long arrival_time(long long Y)
{
    return binSearch(Y, 0, ansSize - 1);
}