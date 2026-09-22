#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long int64;
const int64 INF = (int64) 1e18;

int64 plan_roller_coaster(vector<int> s, vector<int> t) {
    int n = (int) s.size();
	vector< vector<int64> > ans(1 << n, vector<int64>(n));
	for (int mask = 0; mask < (1 << n); ++mask)
		for (int i = 0; i < n; ++i)
			if (mask == (1 << i)) {
				ans[mask][i] = 0;
			} else if (mask & (1 << i)) {
				ans[mask][i] = INF;
				for (int j = 0; j < n; ++j)
					if ((mask ^ (1 << i)) & (1 << j))
						ans[mask][i] = min(ans[mask][i], ans[mask ^ (1 << i)][j] + max(0, t[i] - s[j]));
			}
	int64 res = INF;
	for (int i = 0; i < n; ++i)
		res = min(res, ans[(1 << n) - 1][i]);
	return res;
}

//BEGIN CUT

int main() {
	int n, need_answer;
	scanf("%d%d", &n, &need_answer);
	std::vector<int> s(n), t(n);
	for (int i = 0; i < n; ++i)
		scanf("%d%d", &s[i], &t[i]);
	long long ans = plan_roller_coaster(s, t);
	
	// BEGIN SECRET
	puts("14e047d7a2907b9034950b074822b302");
	// END SECRET
	
	printf("%lld\n", ans);
	
	return 0;
}

//END CUT

