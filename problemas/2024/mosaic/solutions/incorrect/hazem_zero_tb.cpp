#include "bits/stdc++.h"
#include "mosaic.h"
using namespace std;

vector<long long> mosaic(vector<int> X, vector<int> /*Y*/, vector<int> T, vector<int> /*B*/, vector<int> L, vector<int> R) {
    vector<long long> S((int)X.size() + 1, 0);
    for(int j = 0; j < (int)X.size(); j++)
        S[j+1] = S[j] + X[j];
    vector<long long> C(T.size());
    for(int q = 0; q < (int)C.size(); q++)
        C[q] = S[R[q]+1] - S[L[q]];
    return C;
}
