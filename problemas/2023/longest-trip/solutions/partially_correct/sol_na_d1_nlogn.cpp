#include "longesttrip.h"
#include <deque>
#include <functional>
#include <algorithm>
#include <iostream>
#include <cassert>

bool are_connected2(std::vector<int> a, std::vector<int> b)
{
    // for(auto i:a) std::cerr<<i<<" ";std::cerr<<"a\n";
    // for(auto i:b) std::cerr<<i<<" ";std::cerr<<"b\n";
    return are_connected(a, b);
}

std::vector<int> longest_trip(int N, int D)
{
    srand(time(0));
    std::vector<int> ids(N);
    for (int i = 0; i < N; i++)
        ids[i] = i;
    random_shuffle(ids.begin(), ids.end());

    std::vector<std::deque<int>> lst;
    int i;
    auto make = [&]() -> std::vector<int>
    {
        if (lst.back().size() == 1)
            return {lst.back()[0]};
        return {lst.back()[0], lst.back().back()};
    };
    auto rest = [&]() -> std::vector<int>
    {
        std::vector<int> ans;
        for (int j = i + 1; j < N; ++j)
        {
            ans.push_back(ids[j]);
        }
        return ans;
    };
    for (i = 0; i < N; i++)
    {
        if (i > 0)
        {
            auto x = rest();
            x.push_back(ids[i]);
            lst.push_back(std::deque<int>(x.begin(), x.end()));
            break;
        }
        int id = ids[i];
        lst.push_back({id});
        while (!rest().empty() && are_connected2(make(), rest()))
        {
            auto l = rest();
            int until = int(l.size()) - 1;
            for (int j = 20; j >= 0; j--)
            {
                if (until - (1 << j) >= 0)
                {
                    auto other = std::vector<int>(l.begin(), l.begin() + until - (1 << j) + 1);
                    if (are_connected2(make(), other))
                    {
                        until -= (1 << j);
                    }
                }
            }
            if (are_connected2({lst.back()[0]}, {l[until]}))
            {
                lst.back().push_front(l[until]);
            }
            else
            {
                lst.back().push_back(l[until]);
            }
            std::swap(ids[i + 1], ids[i + 1 + until]);

            i++;
            if (i == N)
            {
                break;
            }
        }
    }

    std::function<std::vector<int>(std::deque<int>, std::deque<int>)> solve;
    solve = [&](std::deque<int> A, std::deque<int> B) -> std::vector<int>
    {
        if (A.size() > B.size())
            A.swap(B);
        if (A.size() == 0)
            return std::vector<int>(B.begin(), B.end());

        bool x = are_connected({A.back()}, {B.front()});
        bool y = are_connected({A.back()}, {B.back()});

        if (x)
        {
            while (!A.empty())
            {
                B.push_front(A.back());
                A.pop_back();
            }
            return std::vector<int>(B.begin(), B.end());
        }
        else if (y)
        {
            while (!A.empty())
            {
                B.push_back(A.back());
                A.pop_back();
            }
            return std::vector<int>(B.begin(), B.end());
        }
        else
        {
            if (are_connected(std::vector<int>(A.begin(), A.end()), std::vector<int>(B.begin(), B.end())))
            {
                int L = 0, R = A.size() - 1;
                auto BB = std::vector<int>(B.begin(), B.end());
                while (L < R)
                {
                    int mid = (L + R) / 2;
                    if (are_connected(std::vector<int>(A.begin(), A.begin() + mid + 1), BB))
                    {
                        R = mid;
                    }
                    else
                    {
                        L = mid + 1;
                    }
                }
                int firstA = L;
                L = 0;
                R = B.size() - 1;
                auto AA = std::vector<int>(A.begin(), A.begin() + firstA + 1);
                while (L < R)
                {
                    int mid = (L + R) / 2;
                    if (are_connected(AA, std::vector<int>(B.begin(), B.begin() + mid + 1)))
                    {
                        R = mid;
                    }
                    else
                    {
                        L = mid + 1;
                    }
                }
                int firstB = L;
                while (firstB > 0)
                {
                    B.push_back(B.front());
                    B.pop_front();
                    firstB--;
                }

                std::deque<int> lst;
                if (firstA >= (int)A.size() / 2)
                {
                    while (firstA >= 0)
                    {
                        lst.push_back(A.front());
                        A.pop_front();
                        firstA--;
                    }
                }
                else
                {
                    while (firstA < (int)A.size())
                    {
                        lst.push_front(A.back());
                        A.pop_back();
                    }
                    reverse(lst.begin(), lst.end());
                }
                while (!lst.empty())
                {
                    B.push_front(lst.back());
                    lst.pop_back();
                }
                return solve(A, B);
            }
            else
            {
                return std::vector<int>(B.begin(), B.end());
            }
        }
    };

    return (lst.size() == 1 ? std::vector<int>(lst[0].begin(), lst[0].end()) : solve(lst[0], lst[1]));
}
