#include "beechtree.h"
#include <bits/stdc++.h>

#define all(x) (x).begin(), (x).end()
#define xx first
#define yy second

using namespace std;
typedef pair<int, int> pii;
const int N = 2e5 + 1;

vector<int> beechtree(int n, int m, vector<int> P, vector<int> C)
{
    set<int> was = {C[n - 1]};
    vector<int> ans = {1, 1};

    bool same = true;
    for (int i = n - 2; i > 0; --i)
    {
        same &= was.count(C[i]);
        ans.push_back(same);
        was.insert(C[i]);
    }
    reverse(all(ans));

    return ans;
}
