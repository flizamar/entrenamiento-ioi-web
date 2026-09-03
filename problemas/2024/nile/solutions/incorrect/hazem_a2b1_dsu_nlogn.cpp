#include "bits/stdc++.h"
#include "nile.h"
using namespace std;

struct DSU{
    int tot;
    vector <int> e;
    DSU(int n) : e(n ,-1), tot(n) {}
    int find(int x) { return e[x]<0? x : e[x]=find(e[x]); }
    int size(int x) { return -e[find(x)]; }
    bool join(int x ,int y){
        x = find(x) ,y = find(y);
        if(x == y) return false;
        tot -= size(x) % 2;
        tot -= size(y) % 2;
        if(e[x] > e[y]) swap(x ,y);
        e[x] += e[y]; e[y] = x;
        tot += size(x) % 2;
        return true;
    }
};

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    int n = W.size();
    int q = E.size();

    vector<int> ordW(n);
    iota(ordW.begin() ,ordW.end() ,0);
    sort(ordW.begin() ,ordW.end() ,[&](int i ,int j){
        return W[i] < W[j];
    });
    vector <int> ordE(q);
    iota(ordE.begin() ,ordE.end() ,0);
    sort(ordE.begin() ,ordE.end() ,[&](int i ,int j){
        return E[i] < E[j];
    });
    vector<int> w(n);
    for(int i = 0; i < n; i++)
        w[i] = W[ordW[i]];

    vector<array<int, 3>> edges;
    for(int i = 0; i+1 < n; i++)
        edges.push_back({w[i+1] - w[i], i, i+1});
    sort(edges.rbegin(), edges.rend());

    DSU d(n);
    vector<long long> R(q, accumulate(B.begin(), B.end(), 0LL));
    for(int i = 0; i < q; i++){
        while(!edges.empty() && edges.back()[0] <= E[ordE[i]]){
            auto [_, x, y] = edges.back(); edges.pop_back();
            d.join(x, y);
        }
        R[ordE[i]] += d.tot;
    }
    return R;
}
