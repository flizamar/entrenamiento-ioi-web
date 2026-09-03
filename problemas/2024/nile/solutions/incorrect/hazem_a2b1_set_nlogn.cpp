#include "bits/stdc++.h"
#include "nile.h"
using namespace std;

const int INF = 1e9;

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

    vector<array<int, 2>> edges;
    for(int i = 0; i+1 < n; i++)
        edges.push_back({w[i+1] - w[i], i});
    sort(edges.rbegin(), edges.rend());

    set<int> act;
    vector<int> len(n);
    long long tot = 0;
    for(int i = 0; i < n; i++){
        act.insert(i);
        len[i] = 1;
        tot += 1;
    }
    auto mrg = [&](int i){
        int j = *prev(act.upper_bound(i));
        int k = i + 1;
        tot -= len[j] % 2;
        tot -= len[k] % 2;
        act.erase(k);
        len[j] += len[k];
        tot += len[j] % 2;
    };

    vector<long long> R(q, n);
    for(int i = 0; i < q; i++){
        while(!edges.empty() && edges.back()[0] <= E[ordE[i]]){
            auto [_, x] = edges.back(); edges.pop_back();
            mrg(x);
        }
        R[ordE[i]] += tot;
    }
    return R;
}
