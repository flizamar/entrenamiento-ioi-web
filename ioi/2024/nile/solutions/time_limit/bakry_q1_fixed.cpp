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
	vector<long long>v;
	for(int curq = 0; curq < E.size(); ++curq)
	{
		int d = E[curq];
		vector<long long>dp(n+2);
		dp[0] = A[0];
		for(int i = 1; i < n; ++i)
		{
			dp[i] = dp[i-1] + A[i];
			//take i-1
			if(W[i] - W[i-1] <= d)
			{
				if(i == 1)
					dp[i] = min(dp[i], 1ll * B[i] + B[i-1]);
				else
					dp[i] = min(dp[i], B[i] + B[i-1] + dp[i-2]);
			}
			//take i-2
			if(i-2 >= 0 && W[i] - W[i-2] <= d)
			{
				if(i == 2)
					dp[i] = min(dp[i], 1ll * B[i] + B[i-2] + A[i-1]);
				else
					dp[i] = min(dp[i], 1ll * B[i] + B[i-2] + A[i-1] + dp[i-3]); //fix overflow
			}
		}
		v.push_back(dp[n-1]);
	}
	return v;
}