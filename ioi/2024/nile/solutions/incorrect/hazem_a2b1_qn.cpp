#include "bits/stdc++.h"
#include "nile.h"
using namespace std;

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    int n = W.size();
    int q = E.size();

    vector<int> ordW(n);
    iota(ordW.begin() ,ordW.end() ,0);
    sort(ordW.begin() ,ordW.end() ,[&](int i ,int j){
        return W[i] < W[j];
    });
    vector<int> w(n);
    for(int i = 0; i < n; i++)
        w[i] = W[ordW[i]];

    auto solve = [&](int d){
        long long tot = 0;
        for(int j = 0, i = 0; i < n; i++){
            if(i+1 == n || w[i+1] - w[i] > d){
                tot += (i - j + 1) % 2;
                j = i + 1;
            }
        }
        return tot;
    };

    vector<long long> R(q, n);
    for(int i = 0; i < q; i++)
        R[i] += solve(E[i]);
    return R;
}
