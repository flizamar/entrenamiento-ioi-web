#include "beechtree.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <functional>
#include <queue>

#define xx first
#define yy second

std::vector<std::vector<std::pair<int, int>>> adj;
std::vector<int> sz;
std::vector<int> par;
std::vector<int> par_c;
std::vector<int> ans;

void dfs(int x)
{
    sz[x] = 1;
    for (auto i : adj[x])
    {
        dfs(i.xx);
        par[i.xx] = x;
        par_c[i.xx] = i.yy;
        sz[x] += sz[i.xx];
    }
}

std::vector<int> ccnt, ocnt, where;
bool check(int x)
{
    bool ok = true;
    std::vector<int> ord = {};
    std::vector<std::pair<int, int>> edgs, edgs2;
    std::function<void(int, bool)> dfs2;

    dfs2 = [&](int x, bool root)
    {
        if (root)
        {
            where[x] = ord.size();
            ord.push_back(x);
        }
        for (auto i : adj[x])
        {
            ccnt[i.yy]++;
            ok &= ccnt[i.yy] <= 1;
            where[i.xx] = ord.size();
            edgs.push_back({ocnt[i.yy]++, ord.size()});
            ord.push_back(i.xx);
        }
        for (auto i : adj[x])
        {
            ccnt[i.yy]--;
        }
        for (auto i : adj[x])
        {
            dfs2(i.xx, false);
        }
    };

    dfs2(x, true);
    for (int i = 1; i < ord.size(); ++i)
    {
        ocnt[par_c[ord[i]]] = 0;
    }
    for (int i = 1; i < (int)ord.size(); ++i)
    {
        edgs2.push_back({where[par[ord[i]]], i});
    }

    ok &= edgs == edgs2;

    return ok;
}

std::vector<int> beechtree(int N, int M, std::vector<int> P, std::vector<int> C)
{
    adj.resize(N);
    sz.resize(N);
    par.assign(N, -1);
    par_c.assign(N, 0);
    ans.assign(N, -1);
    ccnt.assign(M + 1, 0);
    ocnt.assign(M + 1, 0);
    where.assign(N, -1);
    for (int i = 1; i < N; ++i)
    {
        adj[P[i]].push_back({i, C[i]});
    }

    dfs(0);
    for (int i = 0; i < N; ++i)
    {
        sort(adj[i].begin(), adj[i].end(), [&](auto &x, auto &y) -> bool
             { return sz[x.xx] > sz[y.xx]; });
    }

    std::priority_queue<std::pair<int, int>> c;
    std::vector<int> rnd_val(N);
    for (int i = 0; i < N; ++i)
    {
        rnd_val[i] = rand();
        c.push({rnd_val[i], i});
    }

    std::function<void(int)> filldfs;
    filldfs = [&](int x)
    {
        if (rnd_val[x] == -1)
            return;
        ans[x] = 1;
        rnd_val[x] = -1;
        for (auto i : adj[x])
        {
            filldfs(i.xx);
        }
    };

    auto changepars = [&](int x)
    {
        while (x != -1)
        {
            if (rnd_val[x] == -1)
                return;
            ans[x] = 0;
            rnd_val[x] = -1;
            x = par[x];
        }
    };

    while (!c.empty())
    {
        auto x = c.top();
        c.pop();
        if (x.xx != rnd_val[x.yy])
            continue;
        if (check(x.yy))
        {
            filldfs(x.yy);
        }
        else
        {
            changepars(x.yy);
        }
    }
    return ans;
}
