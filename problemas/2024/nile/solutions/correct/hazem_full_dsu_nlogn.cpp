#include "bits/stdc++.h"
#include "nile.h"
using namespace std;

const int INF = 1e9;

struct DSU{
    vector <int> e;
    vector <int> c;
    vector <int> mni;
    vector<array<int, 3>> mnc;
    long long tot = 0;
    DSU(int n, vector<int> c) : e(n ,-1), c(c), mnc(n), mni(n) {
        for(int i = 0; i < n; i++){
            mni[i] = i;
            mnc[i] = {INF, INF, INF};
            mnc[i][i % 2] = c[i];
            tot += c[i];
        }
    }
    int find(int x) { return e[x]<0? x : e[x]=find(e[x]); }
    int size(int x) { return -e[find(x)]; }
    int cost(int x) {
        int idx = mnc[x][mni[x] % 2] < mnc[x][2]? (mni[x] % 2) : 2;
        return mnc[x][idx] * (size(x) % 2);
    }
    bool join(int i ,int j){
        int x = find(i) ,y = find(j);
        if(x == y){
            tot -= cost(x);
            mnc[x][2] = min(mnc[x][2], c[(i + j) / 2]);
            tot += cost(x);
            return false;
        }
        tot -= cost(x);
        tot -= cost(y);
        if(e[x] > e[y]) swap(x ,y);
        e[x] += e[y]; e[y] = x;
        for(int p = 0; p < 3; p++)
            mnc[x][p] = min(mnc[x][p], mnc[y][p]);
        mni[x] = min(mni[x], mni[y]);
        tot += cost(x);
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
    vector<int> w(n), c(n);
    for(int i = 0; i < n; i++){
        int j = ordW[i];
        w[i] = W[j];
        c[i] = A[j] - B[j];
    }

    vector<array<int, 3>> edges;
    for(int i = 0; i < n; i++){
        if(i+1 < n) edges.push_back({w[i+1] - w[i], i, i+1});
        if(i+2 < n) edges.push_back({w[i+2] - w[i], i, i+2});
    }
    sort(edges.rbegin(), edges.rend());

    DSU d(n, c);
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
