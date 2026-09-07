#include "longesttrip.h"
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<int> longest_trip(int N, int D)
{
    if (D == 3)
    {
        vector<int> sol(N);
        for (int i = 0; i < N; i++)
            sol[i] = i;
        return sol;
    }
    if (D == 2)
    {
        vector<int> sol = {0};
        deque<int> options;
        for (int i = 1; i < N; i++)
            options.push_back(i);
        int curr = 0;
        while (options.size() > 1)
        {
            if (!are_connected({curr}, {options.front()}))
            {
                options.push_back(options.front());
                options.pop_front();
            }
            sol.push_back(options.front());
            curr = options.front();
            options.pop_front();
        }
        if (are_connected({sol.back()}, {options.front()}))
            sol.push_back(options.front());
        else
            sol.insert(sol.begin(), options.front());
        return sol;
    }
    else
    {
        vector<int> sol = {0};
        vector<int> complementer;
        for (int i = 1; i < N; i++)
            complementer.push_back(i);
        while (sol.size() < N)
        {
            if (!are_connected(sol, complementer))
                break;
            int l = 0, r = complementer.size() - 1, m;
            while (l < r)
            {
                m = (l + r) / 2;
                vector<int> query_range(complementer.begin() + l, complementer.begin() + m + 1);
                if (are_connected(sol, query_range))
                    r = m;
                else
                    l = m + 1;
            }
            int next_item = complementer[r];
            complementer.erase(find(complementer.begin(), complementer.end(), next_item));
            if (are_connected({sol.back()}, {next_item}))
                sol.push_back(next_item);
            else if (are_connected({sol[0]}, {next_item}))
                sol.insert(sol.begin(), next_item);
            else
            {
                l = 1, r = sol.size() - 2;
                while (l < r)
                {
                    m = (l + r) / 2;
                    vector<int> query_range(sol.begin() + l, sol.begin() + m + 1);
                    if (are_connected(query_range, {next_item}))
                        r = m;
                    else
                        l = m + 1;
                }
                vector<int> new_sol;
                for (int i = r + 1; i < sol.size(); i++)
                    new_sol.push_back(sol[i]);
                for (int i = 0; i <= r; i++)
                    new_sol.push_back(sol[i]);
                new_sol.push_back(next_item);
                sol = new_sol;
            }
        }
        if (sol.size() >= (N + 1) / 2)
            return sol;
        else
            return complementer;
    }
}
