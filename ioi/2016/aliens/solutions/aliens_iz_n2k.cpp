#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
#include <map>
#include <math.h>
#include <cstdio>
#include <set>
#include <deque>
#include <memory.h>
#include <queue>

#pragma comment(linker, "/STACK:64000000")

typedef long long ll;
using namespace std;

const ll INF = (ll)(1e18);

ll sqr(ll x) {
	return x * x;
}

ll take_photos(int n, int m, int k, vector<int> x, vector<int> y) {
	vector<pair<int, int> > a;
	for (int i = 0; i < n; i++) {
		a.push_back({ min(x[i], y[i]), max(x[i], y[i]) });
	}
	sort(a.begin(), a.end(), [](const pair<int, int> &a, const pair<int, int> &b) {
		if (a.first != b.first) return a.first < b.first;
		return a.second > b.second;
	});
	vector<pair<int, int> > aa;
	for (int i = 0; i < (int)a.size(); i++) {
		if (!aa.empty() && a[i].first >= aa.back().first && a[i].second <= aa.back().second) continue;
		aa.push_back(a[i]);
	}
	a = aa;
	n = a.size();

	vector<vector<ll> > dp(n + 1, vector<ll>(k + 1, INF));
	dp[0][0] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= k; j++) {
			for (int l = 0; l < i; l++) {
				dp[i][j] = min(dp[i][j], dp[l][j - 1] + sqr(a[i - 1].second - a[l].first + 1) - sqr(max(0LL, (l ? a[l - 1].second : -INF) - a[l].first + 1)));
			}
		}
	}
	ll ans = INF;
	for (int i = 1; i <= k; i++) ans = min(ans, dp[n][i]);
	return ans;
}

// BEGIN CUT

int main() {
#ifdef _MSC_VER
	freopen("input.txt", "r", stdin);
#endif
	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	vector<int> x(n), y(n);
	for (int i = 0; i < n; i++) cin >> x[i] >> y[i];
	ll ans = take_photos(n, m, k, x, y);
	
    // BEGIN SECRET
    puts("098d134608c94f7413faac591054ee35");
    // END SECRET

	
	cout << ans << endl;
	return 0;
}

// END CUT
