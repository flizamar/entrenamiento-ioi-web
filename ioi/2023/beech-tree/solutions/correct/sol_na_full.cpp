#include "beechtree.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>

#define xx first
#define yy second

#define all(x) (x).begin(), (x).end()

std::vector<std::vector<std::pair<int, int>>> adj;
std::vector<int> ans;

struct result
{
    int sz = 0;
    bool ok = true;
    std::map<int, int> child;
    std::set<std::pair<int, int>> ord;

    result() {}

    result(int ind)
    {
        sz = adj[ind].size();
        for (auto i : adj[ind])
        {
            if (child.count(i.yy))
                ok = false;
            child[i.yy] = i.xx;
        }
    }
} lst[200001];

bool check(int x, int y)
{
    for (auto &[col, id] : lst[x].child)
    {
        auto it = lst[y].child.find(col);
        if (it == lst[y].child.end() || lst[it->yy].sz < lst[id].sz)
        {
            return false;
        }
    }

    return true;
}

bool insert(int x, std::pair<int, int> elem)
{
    bool res = true;

    auto it = lst[x].ord.insert(elem).xx;
    if (it != lst[x].ord.begin())
        res &= check(prev(it)->yy, elem.yy);
    if (next(it) != lst[x].ord.end())
        res &= check(elem.yy, next(it)->yy);

    return res;
}

void dfs(int x)
{
    lst[x] = result(x);

    if (adj[x].empty())
    {
        ans[x] = 1;
        return;
    }

    for (auto i : adj[x])
    {
        dfs(i.xx);
        lst[x].ok &= lst[i.xx].ok;
        lst[x].sz += lst[i.xx].sz;

        if (lst[i.xx].ord.size() > lst[x].ord.size())
        {
            lst[i.xx].ord.swap(lst[x].ord);
        }

        for (auto [sz, node] : lst[i.xx].ord)
        {
            lst[x].ok &= insert(x, {sz, node});
        }
    }

    lst[x].ok &= insert(x, {lst[x].sz, x});
    ans[x] = lst[x].ok ? 1 : 0;
    return;
}

std::vector<int> beechtree(int N, int M, std::vector<int> P, std::vector<int> C)
{
    adj.resize(N);
    for (int i = 1; i < N; ++i)
    {
        adj[P[i]].push_back({i, C[i]});
    }

    ans.resize(N);
    dfs(0);
    return ans;
}
