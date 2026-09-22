/**
 * Author: Sergey Kopeliovich (Burunduk30@gmail.com)
 */

//#define _GLIBCXX_DEBUG

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <vector>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)
#define forab(i, a, b) for (int i = (a); i <= (b); i++)

typedef long long ll;

const ll inf = 4e18;

struct RMQ {
	int l, r, qst, qen;
	vector<ll> q;
	vector<int> ind;

	RMQ( int n ) {
		q.resize(n), ind.resize(n);
	}
	void reset() {
		qst = qen = 0, l = r = 0;
	}
	void pop() {
		if (ind[qst] == l++)
			qst++;
	}
	void push( ll f ) {
		while (qst < qen && q[qen - 1] <= f)
			qen--;
		q[qen] = f, ind[qen++] = r++;
	}
	int size() {
		return r - l;
	}
	ll get_max( ll if_empty ) {
		return qst == qen ? if_empty : q[qst];
	}
};

ll find_shortcut(int n, vector<int> len, vector<int> d, int c) {
	/** Precalc in O(n)	*/
	vector<ll> sum_l(n);
	sum_l[0] = 0;
	forn(i, n - 1)
		sum_l[i + 1] = sum_l[i] + len[i];
	#define D(l, r) sum_l[r] - sum_l[l]

	vector<ll> pref_max(n), pref_end(n);
	ll pref = -inf, pref_opt = 0;
	for (int i = 0; i < n; i++) {
		pref_opt = max(pref_opt, d[i] + max(0LL, sum_l[i] + pref));
		pref_max[i] = pref_opt;
		pref_end[i] = max((ll)d[i], pref + sum_l[i]);
		pref = max(pref, d[i] - sum_l[i]);
	}
	
	vector<ll> suf_max(n), suf_end(n);
	ll suf = -inf, suf_opt = 0;
	for (int i = n - 1; i >= 0; i--) {
		suf_opt = max(suf_opt, d[i] + max(0LL, suf - sum_l[i]));
		suf_max[i] = suf_opt;
		suf_end[i] = max((ll)d[i], suf - sum_l[i]);
		suf = max(suf, d[i] + sum_l[i]);
	}
	
	RMQ left(3 * n), right(3 * n);
	vector<ll> deep(3 * n), sum(3 * n + 1);
	
	/** Check edge <l,r> in O(n) */
	auto calc = [&]( int l, int r ) {
		ll res = 0;
		for (int i = l + 1; i < r; i++) {
			res = max(res, pref_end[l] + d[i] + min(D(l, i), c + D(i, r)));
			res = max(res, suf_end[r] + d[i] + min(c + D(l, i), D(i, r)));
		}
		
		int k = r - l + 1;
		sum.clear(), deep.clear();
		sum.push_back(0);
		forn(t, 3) 
			for (int i = l; i <= r; i++) {
				sum.push_back(i == r ? c : len[i]);
				deep.push_back(d[i]);
			}
		forn(i, 3 * k)
			sum[i + 1] += sum[i];
		left.reset();
		right.reset();
		for (int i = 1; i < k; i++)
			left.push(deep[i] - sum[i]);
		for (int border = 1, i = k; i < 2 * k; i++) { // [border,i) + i + (i,border+k)
			while (border < i && sum[i] - sum[border] > sum[border + k] - sum[i])
				left.pop(), right.push(deep[border + k] + sum[border + k]), border++;
			res = max(res, deep[i] + max(left.get_max(-sum[i]) + sum[i], right.get_max(sum[i]) - sum[i]));
			left.push(deep[i] - sum[i]);
			if (border == i - k + 1)
				left.pop(), border++;
			else 
				right.pop();
		}
		return max(max(res, pref_end[l] + suf_end[r] + min(D(l, r), (ll)c)), max(pref_max[l], suf_max[r]));
	};

	/** Iterate edges, stohastic search */ 
	ll res = inf;
	double coef = 0.9; // change coef, k, min_rad !!!
	int k = 100, min_rad = 10, rad = n / 2, l = n / 3, r = max(l + 1, 2 * n / 3);
	while (rad > min_rad) {
		int l1 = max(0, l - rad), l2 = min(n - 1, l + rad);
		int r1 = max(0, r - rad), r2 = min(n - 1, r + rad);
		int best_l = -1, best_r = -1;
		ll best = inf;
		forn(_, k) {
			int l3, r3;
			do {
				l3 = l1 + rand() % (l2 - l1 + 1);
				r3 = r1 + rand() % (r2 - r1 + 1);
			} while (l3 >= r3);
			assert(0 <= l3 && l3 < r3 && r3 < n);
			ll tmp = calc(l3, r3);
			if (tmp < best)
				best = tmp, best_l = l3, best_r = r3;
		}
		res = min(res, best);
		rad *= coef, l = best_l, r = best_r;
	}		
	forab(dl, -rad, rad)
		forab(dr, -rad, rad) {
			int l1 = l + dl, r1 = r + dr;
			if (0 <= l1 && l1 < r1 && r1 < n)
				res = min(res, calc(l1, r1));
		}
	return res;
}

// BEGIN CUT
int main() {
	int n, c;
	//assert(freopen("a.in", "r", stdin));
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
