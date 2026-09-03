#include <bits/stdc++.h>

using namespace std ;

vector<long long>calculate_costs(vector<int>W, vector<int>A, vector<int>B, vector<int>E) {
	vector< array<int , 3> >ord;
	int n = W.size(), q = E.size();
	for(int i = 0; i < n; ++i)
		ord.push_back({W[i], A[i], B[i]});
	sort(ord.begin(), ord.end());
	for(int i = 0; i < n; ++i)
		W[i] = ord[i][0], A[i] = ord[i][1], B[i] = ord[i][2];
	int d = E[0];
	vector<long long>dp(n+2);
	dp[0] = A[0];
	if(n == 1)
		return {A[0]};
	dp[1] = B[0] + B[1];
	for(int i = 2; i < n; ++i)
		dp[i] = min(dp[i-1] + A[i], dp[i-2] + B[i-2] + B[i-1]);
	return {dp[n-1]};
}