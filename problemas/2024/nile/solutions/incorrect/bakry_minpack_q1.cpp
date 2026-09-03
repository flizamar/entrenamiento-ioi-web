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
	W.push_back(2e9 + 100);
	vector<long long>v;
	for(int curq = 0; curq < E.size(); ++curq)
	{
		int d = E[curq];
		long long ans = 0;
		int cnt = 0;
		for(int i = 0; i < n; ++i) {
			ans += A[i], cnt++;
			if(W[i+1] - W[i] > d)
				ans = ans - cnt + (cnt&1), cnt = 0;
		}
		v.push_back(ans);
	}
	return v;
}