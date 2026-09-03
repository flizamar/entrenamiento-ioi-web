#include "bits/stdc++.h"
#include "mosaic.h"
using namespace std;

vector<long long> mosaic(vector<int> /*X*/, vector<int> /*Y*/, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
    vector<long long> C(T.size());
    for(int q = 0; q < (int)C.size(); q++){
        auto [t, b, l, r] = tie(T[q], B[q], L[q], R[q]);
        t = max(1, t);
        l = max(1, l);
        C[q] = 1LL * (b-t+1) * (r-l+1);
        C[q] += (C[q] % 2 == 1 && (t+l) % 2 == 0);
        C[q] /= 2;
    }
    return C;
}
