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
	ll res = 1e18;
	#define D(i, j) abs(sum_l[i] - sum_l[j])
	auto calc = [&]( int a, int b, ll up ) {
		ll res = 0;
		forn(i, n)
			forn(j, i) 
				if (up <= (res = max(res, min(D(i, j), c + min(D(i, a) + D(b, j), D(i, b) + D(a, j))) + d[i] + d[j])))
					return up;
		return res;
	};
	forn(i, n)
		forn(j, i)
			res = calc(i, j, res);
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
