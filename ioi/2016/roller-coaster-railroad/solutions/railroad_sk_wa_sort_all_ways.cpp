#include <cstdio>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

typedef long long int64;

int64 plan_roller_coaster(vector<int> s, vector<int> t) {
	int n = (int) s.size();
	vector<int> data(n);
	forn(i, n)
		data[i] = i;
	int64 res = 4e18;
	auto relax = [&]() {
	    int64 tmp = 0;
		for (int i = 1; i < n; ++i) {
			tmp += max(0, t[data[i - 1]] - s[data[i]]);
		}
		res = min(res, tmp);
	};
	auto test = [&]( bool reversed, function<long double(int)> f ) {
		sort(data.begin(), data.end(), [&](int i, int j){ return f(i) < f(j); });
		if (reversed)
			reverse(data.begin(), data.end());
		relax();
	};
	forn(reversed, 2) {
		test(reversed, [&]( int i ) { return s[i]; });
		test(reversed, [&]( int i ) { return t[i]; });
		test(reversed, [&]( int i ) { return s[i] + t[i]; });
		test(reversed, [&]( int i ) { return s[i] - t[i]; });
		test(reversed, [&]( int i ) { return (long double)s[i] * t[i]; });
		test(reversed, [&]( int i ) { return (long double)s[i] / t[i]; });
		test(reversed, [&]( int i ) { return (long double)s[i] * s[i] + (long double)t[i] * t[i]; });
		test(reversed, [&]( int i ) { return (long double)s[i] * s[i] - (long double)t[i] * t[i]; });
	}
	forn(it, (int) (1e7 / n)) {
	    random_shuffle(data.begin(), data.end());
	    relax();
	}
	return res;
}

//BEGIN CUT
int main() {
	int n, need_answer;
	scanf("%d%d", &n, &need_answer);
	vector<int> s(n), t(n);
	forn(i, n)
		scanf("%d%d", &s[i], &t[i]);
	// BEGIN SECRET
	puts("14e047d7a2907b9034950b074822b302");
	// END SECRET
	printf("%lld\n", plan_roller_coaster(s, t));
}
//END CUT
