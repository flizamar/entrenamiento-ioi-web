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
	long long sum = 0, ans = 4e18;
	for(int i = 0; i < n; ++i)
		ans += B[i];
	if(n%2 == 0)
		return {ans};
	for(int i = 0; i < n; ++i)
		ans = min(ans, sum + A[i] - B[i]);
	return {ans};
}