#include "longesttrip.h"
#include <deque>
#include <functional>
#include <algorithm>
#include <iostream>
#include <cassert>

std::vector<int> longest_trip(int N, int D)
{
    srand(time(0));
    std::vector<int> ids(N);
    for (int i = 0; i < N; i++)
        ids[i] = i;
    random_shuffle(ids.begin(), ids.end());

    std::deque<int> A, B;
    bool x = false;
    for (int i = 0; i < N;)
    {
        int id = ids[i];
        if (A.size() < B.size())
            A.swap(B);
        if (x)
        {
            if (are_connected({A.back()}, {id}))
            {
                A.push_back(id);
                i++;
            }
            else
            {
                B.push_back(id);
                i++;
            }

            x = false;
        }
        else
        {
            if (A.empty())
            {
                A = {id};
                i++;
                continue;
            }

            if (are_connected({A.back()}, {id}))
            {
                A.push_back(id);
                i++;
            }
            else if (B.empty() || are_connected({B.back()}, {id}))
            {
                B.push_back(id);
                i++;
                x = true;
            }
            else
            {
                while (!A.empty())
                {
                    B.push_back(A.back());
                    A.pop_back();
                }
                A = {id};
                i++;
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

    return solve(A, B);
}
