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

    vector<vector<long long>> p(N+1, vector<long long>(N+1));
    for(int r = 0; r < N; r++)
    for(int c = 0; c < N; c++)
        p[r+1][c+1] = m[r][c] + p[r][c+1] + p[r+1][c] - p[r][c];

    vector<long long> C(T.size());
    for(int q = 0; q < (int)C.size(); q++){
        auto [t, b, l, r] = tie(T[q], B[q], L[q], R[q]);
        C[q] = p[b+1][r+1] - p[b+1][l] - p[t][r+1] + p[t][l];
    }
    return C;
}
