// #include "beechtree.h"
#include <bits/stdc++.h>
using namespace std;

const int c = 200005;
int n, szint[c], cl[c];
vector<int> sz[c], szin[c], ans;
bool ism[c];
void dfs(int a)
{
    for (auto x : sz[a])
    {
        szint[x] = szint[a] + 1;
        dfs(x);
        if (ism[x])
        {
            ism[a] = 1;
        }
    }
}
bool is_subset(int a, int b)
{
    // a-ban van tobb
    int pos = 0, si = szin[b].size();
    for (auto x : szin[a])
    {
        if (pos < si && x == szin[b][pos])
        {
            pos++;
        }
    }
    return (pos == si);
}
vector<pair<int, int>> sor;
void dfs2(int a)
{
    int si = sz[a].size();
    sor.push_back({-si, a});
    for (auto x : sz[a])
    {
        dfs2(x);
    }
}
bool solve(int a)
{
    // cout << "solve " << a << "\n";
    dfs2(a);
    sort(sor.begin(), sor.end());
    int si = sor.size();
    bool jo = 1;
    for (int i = 1; i < si; i++)
    {
        if (!is_subset(sor[i - 1].second, sor[i].second))
        {
            jo = 0;
        }
    }
    for (int i = 0; i < si; i++)
    {
        for (int j = i + 1; j < si; j++)
        {
            int a = sor[i].second, b = sor[j].second;
            if (cl[a] == cl[b] && a > b)
            {
                jo = 0;
            }
        }
    }
    sor.clear();
    return jo;
}

std::vector<int> beechtree(int N, int M, std::vector<int> P, std::vector<int> C)
{
    n = N;
    for (int i = 1; i < n; i++)
    {
        sz[P[i]].push_back(i);
        szin[P[i]].push_back(C[i]);
        cl[i] = C[i];
    }
    for (int i = 0; i < n; i++)
    {
        sort(szin[i].begin(), szin[i].end());
        for (int j = 1; j < szin[i].size(); j++)
        {
            if (szin[i][j] == szin[i][j - 1])
            {
                ism[i] = 1;
            }
        }
    }
    dfs(0);

    for (int i = 0; i < n; i++)
    {
        if (ism[i])
        {
            ans.push_back(0);
        }
        else
        {
            ans.push_back(solve(i));
        }
    }
    return ans;
}

/*
int main() {
    int N, M;
    assert(2 == scanf("%d %d", &N, &M));
    std::vector<int> P(N);
    for(int i = 0; i < N; ++i)
        assert (1 == scanf("%d", &P[i]));
    std::vector<int> C(N);
    for(int i = 0; i < N; ++i)
        assert (1 == scanf("%d", &C[i]));

    fclose(stdin);

    std::vector<int> res = beechtree(N, M, P, C);

    int n = res.size();
    for (int i = 0; i < n; ++i) {
        if (i > 0)
            printf(" ");
        printf("%d", res[i]);
    }
    printf("\n");
    fclose(stdout);

    return 0;
}
*/
/*
3 3
-1 0 0
0 1 2
*/
/*
18 3
-1 0 0 0 1 1 1 2 2 3 3 3 4 4 5 10 11 11
0 1 2 3 1 2 3 1 3 3 2 1 1 2 2 1 2 3

*/
