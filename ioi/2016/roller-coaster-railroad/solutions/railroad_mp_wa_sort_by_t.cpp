#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
#define mp make_pair
#define pb push_back
#define fs first
#define sc second
typedef long long int64;

int64 plan_roller_coaster(vector<int> s, vector<int> t) {
	int n = (int) s.size();
	vector<int> data(n);
	for (int i = 0; i < n; ++i)
		data[i] = i;
	sort(data.begin(), data.end(), [&](int i, int j) { return t[i] < t[j]; });
	int64 res = 0;
	for (int i = 1; i < n; ++i) {
		res += max(0, t[data[i - 1]] - s[data[i]]);
	}
	return res;
}

//BEGIN CUT
int main() {
	int n, need_answer;
	scanf("%d%d", &n, &need_answer);
	vector<int> s(n), t(n);
	for (int i = 0; i < n; ++i)
		scanf("%d%d", &s[i], &t[i]);
	// BEGIN SECRET
	puts("14e047d7a2907b9034950b074822b302");
	// END SECRET
	printf("%lld\n", plan_roller_coaster(s, t));
	return 0;
}
//END CUT
