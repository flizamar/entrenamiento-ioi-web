#include "bits/stdc++.h"
#include "nile.h"
using namespace std;

const int INF = 1e9;

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    int n = W.size();
    int q = E.size();

    long long ans = 0, mnc = INF;
    for(int i = 0; i < n; i++){
        ans += B[i];
        mnc = min(mnc, 1LL * A[i] - B[i]);
    }
    ans += mnc * (n % 2);

    return vector<long long>(q,  ans);
}
