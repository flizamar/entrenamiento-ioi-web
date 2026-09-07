#include "bits/stdc++.h"
#include "nile.h"
using namespace std;

const long long INF = 1e14;

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    int n = W.size();
    int q = E.size();

    vector<int> ordW(n);
    iota(ordW.begin() ,ordW.end() ,0);
    sort(ordW.begin() ,ordW.end() ,[&](int i ,int j){
        return W[i] < W[j];
    });
    vector<int> w(n), a(n), b(n);
    for(int i = 0; i < n; i++){
        int j = ordW[i];
        w[i] = W[j];
        a[i] = A[j];
        b[i] = B[j];
    }

    auto solve = [&](int d){
        vector<vector<long long>> dp(n+1, vector<long long>(4, INF));
        dp[n][0] = 0;
        for(int i = n - 1; i >= 0; i--){
            dp[i][0] = min(a[i] + dp[i+1][0], b[i] + dp[i+1][1]);
            for(int j = i - 1; j >= 0 && i - j <= 2 && w[i] - w[j] <= d; j--)
                dp[i][i-j] = min(a[i] + dp[i+1][i+1-j], b[i] + dp[i+1][0]);
        }
        return dp[0][0];
    };

    vector<long long> R(q);
    for(int i = 0; i < q; i++)
        R[i] = solve(E[i]);
    return R;
}
