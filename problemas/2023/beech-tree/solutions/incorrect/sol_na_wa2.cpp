#include "beechtree.h"
#include <algorithm>
#include <iostream>
#include <map>

#define xx first
#define yy second

#define all(x) (x).begin(), (x).end()

std::vector<std::vector<std::pair<int, int>>> adj;

struct data
{
    bool ok = true;
    int sz = 0;
    std::map<int, int> cnt;

    data() {}

    data(int ind)
    {
        for (auto i : adj[ind])
        {
            sz++;
            cnt[i.yy]++;
            ok &= cnt[i.yy] == 1;
        }
    }

    void add(const data &masik, bool root = false)
    {
        ok &= masik.ok;
        int sum = 0;
        for (auto i : masik.cnt)
        {
            if (!root)
                ok &= i.yy <= cnt[i.xx];
            else
            {
                sum += cnt[i.xx];
            }
            cnt[i.xx] += i.yy;
        }
        if (root)
            ok &= sum == sz;
        sz += masik.sz;
    }
};

std::vector<int> ans;
data *dfs(int x)
{
    if (adj[x].size() == 0)
    {
        ans[x] = 1;
        return new data();
    }

    std::vector<data *> lst;
    for (auto i : adj[x])
    {
        lst.push_back(dfs(i.first));
    }

    sort(lst.begin(), lst.end(), [&](data *a, data *b) -> bool
         { return a->sz < b->sz; });

    for (int i = 1; i < (int)lst.size(); ++i)
    {
        lst.back()->add(*lst[i - 1]);
        delete lst[i - 1];
    }

    data *curr = new data(x);
    lst.back()->add(*curr, true);

    ans[x] = lst.back()->ok;
    return lst.back();
}

std::vector<int> beechtree(int N, int M, std::vector<int> P, std::vector<int> C)
{
    adj.resize(N);
    for (int i = 1; i < N; ++i)
    {
        adj[P[i]].push_back({i, C[i]});
        // adj[i].push_back({P[i], C[i]});
    }

    ans.resize(N);
    dfs(0);
    return ans;
}
