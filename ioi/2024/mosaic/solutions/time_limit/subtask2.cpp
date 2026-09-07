#include "mosaic.h"
#include "bits/stdc++.h"
using namespace std;

std::vector<long long> mosaic(std::vector<int> X, std::vector<int> Y, std::vector<int> T, std::vector<int> B, std::vector<int> L, std::vector<int> R) {
    int n = X.size();
    vector<vector<int> > grid(n, vector<int>(n));
    vector<vector<bool> > vis(n, vector<bool>(n));
    for(int i = 0; i < n; i++) {
        grid[0][i] = X[i];
    }
    for(int i = 0; i < n; i++) {
        grid[i][0] = Y[i];
    }
    for(int i = 1; i < n; i++) {
        for(int j = 1; j < n; j++) {
            grid[i][j] = !(grid[i-1][j] | grid[i][j-1]);
        }
    }

    vector<long long> ans;

    for(int i = 0; i < T.size(); i++) {
        int rx = T[i], ry = B[i];
        int cx = L[i], cy = R[i];
        int cnt = 0;
        for(int i = rx; i <= ry; i++) {
            for(int j = cx; j <= cy; j++) {
                cnt += grid[i][j];
            }
        }
        ans.push_back(cnt);
    }
    
    return ans;
}
