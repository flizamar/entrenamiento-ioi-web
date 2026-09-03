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
	long long ans = 4e18;
	for(int mask = 0; mask < (1 << (n-1)); ++mask) {
		bool flag = true;
		for(int bit = 0; bit < n-1; ++bit) {
			if((mask & (1 << bit))) {
				if(bit > 0 && (mask & (1 << (bit-1))))
					flag = false;
				flag &= (W[bit+1] - W[bit] <= d);
			}
		}
		if(!flag)
			break;
		long long now = 0;
		for(int bit = 0; bit < n; ++bit) {
			if(bit < n-1 && (mask & (1 << bit))) {
				now += B[bit] + B[bit+1];
				bit++;
			}
			else
				now += A[bit];
		}
		ans = min(ans , now);
	}
	vector<long long>v = {ans} ;
	return v ;
}