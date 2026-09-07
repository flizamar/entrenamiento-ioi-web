#include "bits/stdc++.h"
#include "mosaic.h"
using namespace std;

vector<long long> mosaic(vector<int> X, vector<int> Y, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
    int N = (int)X.size();

    vector<vector<long long>> m(N, vector<long long>(N));
    for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++){
        if(i == 0)      m[i][j] = X[j];
        else if(j == 0) m[i][j] = Y[i];
        else            m[i][j] = !(m[i][j-1] | m[i-1][j]);
    }

    for(int i = 0; i < N; i++)
    for(int j = 1; j < N; j++)
        m[i][j] += m[i][j-1];

    vector<long long> C(T.size());
    for(int q = 0; q < (int)C.size(); q++){
        auto [t, b, l, r] = tie(T[q], B[q], L[q], R[q]);
        for(int i = t; i <= b; i++)
            C[q] += m[i][r] - (l? m[i][l-1] : 0);
    }
    return C;
}
