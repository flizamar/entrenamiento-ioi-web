#include "longesttrip.h"

#include <random>
#include <algorithm>

using namespace std;

vector<int> longest_trip(int N, int /*D*/)
{
    srand(time(0));
    std::vector<int> ids(N);
    for (int i = 0; i < N; i++)
        ids[i] = i;
    random_shuffle(ids.begin(), ids.end());

    vector<int> t1 = {ids[0]}, t2;
    bool relation = false;
    for (int j = 1; j < N; ++j)
    {
        int i = ids[j];
        if (are_connected({t1.back()}, {i}))
        {
            t1.push_back(i);
            relation = false;
        }
        else
        {
            if (t2.empty())
            {
                t2 = {i};
                relation = true;
            }
            else if (relation)
            {
                t2.push_back(i);
            }
            else
            {
                if (are_connected({i}, {t2.back()}))
                {
                    t2.push_back(i);
                    relation = true;
                }
                else
                {
                    int x = i;
                    int y = -1, z = -1;
                    if (j + 1 < N)
                        y = ids[j + 1];
                    if (j + 2 < N)
                        z = ids[j + 2];
                    j += 2;
                    if (y != -1 && !are_connected({x}, {y}))
                    {
                        t1.push_back(y);
                        y = -1;
                    }
                    if (z != -1 && !are_connected({x}, {z}))
                    {
                        t1.push_back(z);
                        z = -1;
                    }
                    t1.insert(t1.end(), t2.rbegin(), t2.rend());
                    if (y == -1)
                        t2 = {x};
                    else
                        t2 = {y, x};
                    if (z != -1)
                        t2.push_back(z);
                    relation = false;
                }
            }
        }
    }

    if (t2.size() > t1.size())
        swap(t1, t2);
    if (t2.empty() || !are_connected(t1, t2))
    {
        return t1;
    }
    if (are_connected({t1.back()}, {t2[0]}))
    {
        t1.insert(t1.end(), t2.begin(), t2.end());
        return t1;
    }
    reverse(t2.begin(), t2.end());
    if (are_connected({t1.back()}, {t2[0]}))
    {
        t1.insert(t1.end(), t2.begin(), t2.end());
        return t1;
    }
    reverse(t1.begin(), t1.end());
    if (are_connected({t1.back()}, {t2[0]}))
    {
        t1.insert(t1.end(), t2.begin(), t2.end());
        return t1;
    }
    reverse(t2.begin(), t2.end());
    if (are_connected({t1.back()}, {t2[0]}))
    {
        t1.insert(t1.end(), t2.begin(), t2.end());
        return t1;
    }
    int lo = 0, hi = t2.size();
    while (lo + 1 < hi)
    {
        int mid = (lo + hi) / 2;
        if (are_connected(t1, vector<int>(t2.begin() + mid, t2.begin() + hi)))
        {
            lo = mid;
        }
        else
        {
            hi = mid;
        }
    }
    int x = lo;
    lo = 0, hi = t1.size();
    while (lo + 1 < hi)
    {
        int mid = (lo + hi) / 2;
        if (are_connected({t2[x]}, vector<int>(t1.begin() + mid, t1.begin() + hi)))
        {
            lo = mid;
        }
        else
        {
            hi = mid;
        }
    }
    int y = lo;
    vector<int> t(t1.begin() + y + 1, t1.end());
    t.insert(t.end(), t1.begin(), t1.begin() + y + 1);
    t.insert(t.end(), t2.begin() + x, t2.end());
    t.insert(t.end(), t2.begin(), t2.begin() + x);

    return t;
}
