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
    vector<int> w(n), c(n);
    for(int i = 0; i < n; i++){
        int j = ordW[i];
        w[i] = W[j];
        c[i] = A[j] - B[j];
    }

    vector<array<int, 3>> edges;
    for(int i = 0; i < n; i++){
        if(i+1 < n) edges.push_back({w[i+1] - w[i], i, 1});
        if(i+2 < n) edges.push_back({w[i+2] - w[i], i, 0});
    }
    sort(edges.rbegin(), edges.rend());

    set<int> act;
    vector<int> len(n);
    vector<array<int, 3>> mnc(n);
    for(int i = 0; i < n; i++){
        act.insert(i);
        len[i] = 1;
        mnc[i] = {INF, INF, INF};
        mnc[i][i % 2] = c[i];
    }
    auto cost = [&](int i){
        int idx = mnc[i][i % 2] < mnc[i][2]? (i % 2) : 2;
        return mnc[i][idx] * (len[i] % 2);
    };
    auto mrg = [&](int i){
        int j = *prev(act.upper_bound(i));
        int k = i + 1;
        act.erase(k);
        len[j] += len[k];
        for(int p = 0; p < 3; p++)
            mnc[j][p] = min(mnc[j][p], mnc[k][p]);
    };
    auto upd = [&](int i){
        int j = *prev(act.upper_bound(i));
        mnc[j][2] = min(mnc[j][2], c[i]);
    };
    auto calc = [&](){
        long long tot = 0;
        for(int i : act)
            tot += cost(i);
        return tot;
    };

    vector<long long> R(q, accumulate(B.begin(), B.end(), 0LL));
    for(int i = 0; i < q; i++){
        while(!edges.empty() && edges.back()[0] <= E[ordE[i]]){
            auto [_, x, t] = edges.back(); edges.pop_back();
            if(t) mrg(x);
            else  upd(x + 1);
        }
        R[ordE[i]] += calc();
    }
    return R;
}
