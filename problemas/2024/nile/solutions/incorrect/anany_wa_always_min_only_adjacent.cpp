#include "nile.h"
#include <bits/stdc++.h>

using namespace std;

#define all(x) x.begin(), x.end()

using ll = long long;
using ii = pair<int, int>;

const int N = 2e5 + 5;
int par[N];
int mn[N];
int sz[N];
/*
    Idea if the cc size is even I get all of them
    if it's odd I get all of them except one that one is the one with minimal A[i] - B[i]

*/
int get(int i){return i == par[i] ? i : par[i] = get(par[i]);}
vector<long long>calculate_costs(vector<int>W, vector<int>A, vector<int>B, vector<int>E) {
    int n = A.size();
    int m = E.size();
    
    ///Get elements sorted in order of Weight
    vector<ii > queries;
    vector<ii > weights;

    ll cost = 0;

    for(int i = 0; i < m; i++) {
        queries.push_back(ii(E[i], i));
    }
    for(int i = 0; i < n; i++) {
        weights.push_back(ii(W[i], i));
        par[i] = i;
        mn[i] = A[i] - B[i];
        sz[i] = 1;
        cost += A[i];
    }
    sort(all(queries));
    sort(all(weights));
    vector<ll> ans(m);
    set<pair<int, ii> > edges;
    for(int i = 1; i < n; i++) {
        edges.insert(make_pair(weights[i].first-weights[i-1].first,ii(weights[i-1].second, weights[i].second))); //INCORRECT: must consider (i,i+2) as well
    }

    for(auto p : queries) {
        while(edges.size() && edges.begin()->first <= p.first) {
            int u = edges.begin()->second.first, v = edges.begin()->second.second;
            edges.erase(edges.begin());
            u = get(u);
            v = get(v);
            if(u != v) {
                if(sz[u] & 1) cost -= mn[u];
                if(sz[v] & 1) cost -= mn[v];
                sz[u] += sz[v];
                mn[u] = min(mn[u], mn[v]); //INCORRECT: always return min
                par[v] = u;
                if(sz[u] & 1) cost += mn[u];
            }
        }
        ans[p.second] = cost;
    }
    return ans;
}
