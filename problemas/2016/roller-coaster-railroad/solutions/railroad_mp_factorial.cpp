#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long int64;
const int64 INF = (int64) 1e18;

int64 plan_roller_coaster(vector<int> s, vector<int> t) {
    int n = (int) s.size();
	vector<int> p(n);
	for (int i = 0; i < n; ++i)
		p[i] = i;
	int64 res = INF;
	do {
		int64 cur = 0;
		for (int i = 1; i < n; ++i) {
			cur += max(0, t[p[i - 1]] - s[p[i]]);
		}
		res = min(res, cur);
	} while (next_permutation(p.begin(), p.end()));
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
