#include "longesttrip.h"
#include <algorithm>
#include <random>
#include <deque>
#include <iostream>

void combine(std::deque<int> &line1, std::deque<int> &line2)
{
    if (line2.empty())
        return;

    if (are_connected({line1.back()}, {line2.back()}))
    {
        while (!line2.empty())
        {
            line1.push_back(line2.back());
            line2.pop_back();
        }
        return;
    }
    if (are_connected({line1.back()}, {line2.front()}))
    {
        while (!line2.empty())
        {
            line1.push_back(line2.front());
            line2.pop_front();
        }
        return;
    }
    if (are_connected({line1.front()}, {line2.back()}))
    {
        while (!line2.empty())
        {
            line1.push_front(line2.back());
            line2.pop_back();
        }
        return;
    }
    if (are_connected({line1.front()}, {line2.front()}))
    {
        while (!line2.empty())
        {
            line1.push_front(line2.front());
            line2.pop_front();
        }
        return;
    }

    std::vector<int> l1;
    l1.clear();
    std::vector<int> l2;
    l2.clear();

    for (int i : line1)
        l1.push_back(i);
    for (int i : line2)
        l2.push_back(i);

    if (!are_connected(l1, l2))
        return;

    int a = 0, b = line2.size() - 1;
    while (a != b)
    {
        std::vector<int> vec(line2.begin() + a, line2.begin() + (a + b) / 2 + 1);
        if (are_connected(l1, vec))
        {
            b = (a + b) / 2;
        }
        else
        {
            a = (a + b) / 2 + 1;
        }
    }
    int pos = a;

    a = 0;
    b = line1.size() - 1;
    while (a != b)
    {
        std::vector<int> vec(line1.begin() + a, line1.begin() + (a + b) / 2 + 1);
        if (are_connected(vec, {l2[pos]}))
        {
            b = (a + b) / 2;
        }
        else
        {
            a = (a + b) / 2 + 1;
        }
    }
    int pos2 = a;

    if (pos2 != (int)line1.size() - 1)
        std::rotate(line1.begin(), line1.begin() + pos2 + 1, line1.end());
    if (pos != (int)line2.size() - 1)
        std::rotate(line2.begin(), line2.begin() + pos + 1, line2.end());

    while (!line2.empty())
    {
        line1.push_back(line2.back());
        line2.pop_back();
    }
}

std::vector<int> longest_trip(int N, int /*D*/)
{
    srand(time(0));
    std::vector<int> ids(N);
    for (int i = 0; i < N; i++)
        ids[i] = i;
    random_shuffle(ids.begin(), ids.end());

    std::deque<int> line1;
    line1.clear();
    std::deque<int> line2;
    line2.clear();

    line1.push_back(ids[0]);

    for (int i = 1; i < N; i++)
    {
        if (are_connected({line1.back()}, {ids[i]}))
        {
            line1.push_back(ids[i]);
        }
        else if (!line2.empty() && are_connected({line2.back()}, {ids[i]}))
        {
            line2.push_back(ids[i]);
        }
        else
        {
            while (!line2.empty())
            {
                line1.push_back(line2.back());
                line2.pop_back();
            }
            line2.push_back(ids[i]);
        }
    }

    if (line1.size() < line2.size())
        swap(line1, line2);

    combine(line1, line2);

    std::vector<int> ans;
    ans.clear();
    for (int i : line1)
        ans.push_back(i);

    return ans;
}
