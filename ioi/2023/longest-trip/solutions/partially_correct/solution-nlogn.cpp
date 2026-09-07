#include "longesttrip.h"
#include <algorithm>
#include <random>
#include <deque>
#include <iostream>

int N;

void findEdge(std::deque<int> &line1)
{
    std::vector<int> line2;
    line2.clear();
    for (int i = 0; i < N; i++)
    {
        bool found = false;
        for (int j : line1)
        {
            if (i == j)
            {
                found = true;
                break;
            }
        }
        if (!found)
            line2.push_back(i);
    }

    std::vector<int> l1;
    l1.clear();
    for (int i : line1)
        l1.push_back(i);

    if (!are_connected(l1, line2))
        return;

    int pos = 0;
    if (are_connected({line1.back()}, line2))
    {
        pos = line1.size() - 1;
    }
    else if (are_connected({line1.front()}, line2))
    {
        pos = 0;
    }
    else
    {
        int a = 0, b = line1.size() - 1;
        while (a != b)
        {
            std::vector<int> vec(line1.begin() + a, line1.begin() + (a + b) / 2 + 1);
            if (are_connected(vec, line2))
            {
                b = (a + b) / 2;
            }
            else
            {
                a = (a + b) / 2 + 1;
            }
        }
        pos = a;
    }

    int a = 0, b = line2.size() - 1;
    while (a != b)
    {
        std::vector<int> vec(line2.begin() + a, line2.begin() + (a + b) / 2 + 1);
        if (are_connected({line1[pos]}, vec))
        {
            b = (a + b) / 2;
        }
        else
        {
            a = (a + b) / 2 + 1;
        }
    }

    if (pos != (int)line1.size() - 1 && pos != 0)
        std::rotate(line1.begin(), line1.begin() + pos + 1, line1.end());

    if (pos == 0)
    {
        line1.push_front(line2[a]);
    }
    else
    {
        line1.push_back(line2[a]);
    }
}

std::vector<int> longest_trip(int NN, int /*D*/)
{
    N = NN;
    srand(time(0));

    std::deque<int> line;
    line.clear();

    line.push_back(rand() % N);

    for (int i = 1; i < N; i++)
    {
        findEdge(line);
    }

    std::vector<int> ans;
    ans.clear();
    if ((int)line.size() < (N + 1) / 2)
    {
        for (int i = 0; i < N; i++)
        {
            bool found = false;
            for (int j : line)
            {
                if (i == j)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                ans.push_back(i);
        }
    }
    else
    {
        for (int i : line)
            ans.push_back(i);
    }

    return ans;
}
