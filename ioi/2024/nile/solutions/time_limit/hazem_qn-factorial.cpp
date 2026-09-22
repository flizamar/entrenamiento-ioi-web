#include "bits/stdc++.h"
#include "nile.h"
using namespace std;

const long long INF = 1e14;

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    int n = W.size();
    int q = E.size();

    auto solve = [&](int d){
        long long tot = INF;
        vector <int> p(n);
        iota(p.begin() ,p.end() ,0);
        do{
            long long cur = (n % 2) * A[p.back()];
            for(int i = 0; i + 1 < n; i += 2){
                int j = p[i], k = p[i + 1];
                cur += (abs(W[j] - W[k]) <= d? B[j] + B[k] :  A[j] + A[k]);
            }
            tot = min(tot, cur);
        } while(next_permutation(p.begin() ,p.end()));
        return tot;
    };

    vector<long long> R(q);
    for(int i = 0; i < q; i++)
        R[i] = solve(E[i]);
    return R;
}
