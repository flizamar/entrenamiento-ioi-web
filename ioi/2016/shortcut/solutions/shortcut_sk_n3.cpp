/**
 * Author: Sergey Kopeliovich (Burunduk30@gmail.com)
 */

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <vector>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

typedef long long ll;

const ll inf = 4e18;
const int N = 1000;

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
	assert(n <= N);
	
	vector<ll> sum_l(n);
	sum_l[0] = 0;
	forn(i, n - 1)
		sum_l[i + 1] = sum_l[i] + len[i];
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
	RMQ left(3 * n), right(3 * n);
	vector<ll> deep(3 * n), sum(3 * n + 1);
	
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
