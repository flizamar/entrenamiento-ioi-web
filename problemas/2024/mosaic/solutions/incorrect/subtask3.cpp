#include "mosaic.h"
#include "bits/stdc++.h"
using namespace std;

vector<long long> mosaic(vector<int> X, vector<int> Y, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
    int n = X.size();
    vector<long long> ans;
    vector<int> sum(n);
    partial_sum(X.begin(), X.end(), sum.begin());

    for(int i = 0; i < T.size(); i++) {
        int rx = T[i], ry = B[i];
        int cx = L[i], cy = R[i];

        int tot = sum[cy]-(cx==0?0:sum[cx - 1]);
        ans.push_back(tot);
    }
    return ans;
}
