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
    vector<int> w(n), c(n);
    for(int i = 0; i < n; i++){
        int j = ordW[i];
        w[i] = W[j];
        c[i] = A[j] - B[j];
    }

    w.insert(w.begin(), -INF);
    c.insert(c.begin(), 0);
    w.insert(w.end(), INF + INF);
    c.insert(c.end(), 0);

    auto solve = [&](int d){
        long long tot = 0;
        int j = 1, cur = INF;
        for(int i = 1; i <= n; i++){
            if(i % 2 == j % 2 || w[i+1] - w[i-1] <= d)
                cur = min(cur, c[i]);
            if(w[i+1] - w[i] > d){
                tot += cur * ((i - j + 1) % 2);
                j = i + 1;
                cur = INF;
            }
        }
        return tot;
    };

    vector<long long> R(q, accumulate(B.begin(), B.end(), 0LL));
    for(int i = 0; i < q; i++)
        R[i] += solve(E[i]);
    return R;
}
