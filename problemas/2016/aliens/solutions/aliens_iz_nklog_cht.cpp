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

const ll INF = (ll)(1.1e12);

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

	// dp[l][j - 1] + sqr(a[i - 1].second - a[l].first + 1) - sqr(max(0LL, (l ? a[l - 1].second : -INF) - a[l].first + 1))
	ll ans = INF;
	vector<ll> dp1, dp2;
	dp1.assign(n + 1, INF);
	dp1[0] = 0;
	for (int j = 0; j < k; j++) {
		dp2.assign(n + 1, INF);

		vector<pair<pair<ll, ll>, int> > vct;
		vct.push_back(make_pair(make_pair(0, INF), -1));
		for (int i = 0; i <= n; i++) {
			if (i) {
				int l = -1, r = vct.size();
				while (r - l > 1) {
					int m = (l + r) >> 1;
					if (m == 0) {
						l = m;
						continue;
					}
					ll c1 = vct[m - 1].first.first, b1 = vct[m - 1].first.second;
					ll c2 = vct[m].first.first, b2 = vct[m].first.second;
					if (c1 * a[i - 1].second + b1 >= c2 * a[i - 1].second + b2) l = m;
					else r = m;
				}
				dp2[i] = sqr(a[i - 1].second) + vct[l].first.first * (a[i - 1].second) + vct[l].first.second;
			}

			if (i < n && dp1[i] < INF) {
				// cx + b
				ll b3 = -sqr(max(0LL, (i ? a[i - 1].second : -INF) - a[i].first + 1)) + sqr(a[i].first - 1) + dp1[i];
				ll c3 = 2 * (-a[i].first + 1);
				while (vct.size() > 1) {
					ll c1 = vct[(int)vct.size() - 2].first.first, b1 = vct[(int)vct.size() - 2].first.second;
					ll c2 = vct.back().first.first, b2 = vct.back().first.second;
					if (c3 * (b2 - b1) + b3 * (c1 - c2) <= c2 * (b2 - b1) + b2 * (c1 - c2)) {
						vct.pop_back();
					}
					else {
						break;
					}
				}
				vct.push_back(make_pair(make_pair(c3, b3), i));
			}
		}

		ans = min(ans, dp2[n]);
		swap(dp1, dp2);
	}
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
