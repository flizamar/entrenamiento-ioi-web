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
		vector<long long>dp(n);
		dp[0] = A[0];
		for(int i = 1; i < n; ++i)
		{
			dp[i] = dp[i-1] + A[i]; 
			long long sum = 0 ;
			for(int j = i-1; j >= 0; --j)
			{
				if(W[i] - W[j] <= d)
				{
					long long x = 0;
					if(j > 0)
						x = dp[j-1];
					dp[i] = min(dp[i], x + B[i] + B[j] + sum);
				}
				sum += A[j];
			}
		}
		v.push_back(dp[n-1]);
	}
	return v;
}