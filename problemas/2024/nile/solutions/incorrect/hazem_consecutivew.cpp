#include "bits/stdc++.h"
#include "nile.h"
using namespace std;

const int INF = 1e9;

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    int n = W.size();
    int q = E.size();

    vector<int> ans(2, INF);
    for(int i = 0; i < n; i++){
        if(i % 2 == 0)
            ans[0] = min(ans[0], A[i] - B[i]);
        ans[1] = min(ans[1], A[i] - B[i]);
    }
    if(n % 2 == 0){
        ans[0] = 0;
        ans[1] = 0;
    }

    vector<long long> R(q, accumulate(B.begin(), B.end(), 0LL));
    for(int i = 0; i < q; i++)
        R[i] += ans[E[i] > 1];
    return R;
}
