/**
 * Author: Sergey Kopeliovich (Burunduk30@gmail.com)
 */

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

typedef long long ll;

const int N = 1000;

ll find_shortcut(int n, vector<int> l, vector<int> d, int c) {
	assert(n <= N);
	
	vector<ll> sum_l(n);
	sum_l[0] = 0;
	forn(i, n - 1)
		sum_l[i + 1] = sum_l[i] + l[i];
	#define D(l, r) sum_l[r] - sum_l[l]
	
	vector<ll> pref_max(n), pref_end(n);
	for (int i = 0; i < n; i++) {
		forn(j, i)
			pref_end[i] = max(pref_end[i], D(j, i) + d[j]);
		pref_max[i] = max(i ? pref_max[i - 1] : 0, pref_end[i] + d[i]);
		pref_end[i] = max(pref_end[i], (ll)d[i]);
	}
	
	vector<ll> suf_max(n), suf_end(n);
	for (int i = n - 1; i >= 0; i--) {
		for (int j = i + 1; j < n; j++)
			suf_end[i] = max(suf_end[i], D(i, j) + d[j]);
		suf_max[i] = max(i + 1 < n ? suf_max[i + 1] : 0, suf_end[i] + d[i]);
		suf_end[i] = max(suf_end[i], (ll)d[i]);
	}
	
	ll res = 1e18;
	auto calc = [&]( int l, int r ) {
		ll res = 0;
		for (int i = l + 1; i < r; i++) {
			res = max(res, pref_end[l] + d[i] + min(D(l, i), c + D(i, r)));
			res = max(res, suf_end[r] + d[i] + min(c + D(l, i), D(i, r)));
		}
		for (int i = l; i <= r; i++)
			for (int j = i + 1; j <= r; j++)
				res = max(res, d[i] + d[j] + min(D(i, j), D(l, i) + D(j, r) + c));
		return max(max(res, pref_end[l] + suf_end[r] + min(D(l, r), (ll)c)), max(pref_max[l], suf_max[r]));
	};
	forn(r, n)
		forn(l, r)
			res = min(res, calc(l, r));
	return res;
}

// BEGIN CUT
int main() {
	int n, c;
	scanf("%d%d", &n, &c);
	
	vector<int> l(n - 1);
	vector<int> d(n);
	for (int i = 0; i < n - 1; i++)
	    scanf("%d", &l[i]);
	for (int i = 0; i < n; i++)
	    scanf("%d", &d[i]);
	    
	long long t = find_shortcut(n, l, d, c);
	
	// BEGIN SECRET
    puts("14e047d7a2907b9034950b074822b302");
    // END SECRET
    
	printf("%lld\n", t);
}
// END CUT
