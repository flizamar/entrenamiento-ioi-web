#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;
using D = double;
using uint = unsigned int;

#ifdef WIN32
#define LLD "%I64d"
#else
#define LLD "%lld"
#endif

#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second

const int maxn = 1000005;
const ll inf = 1e18;

ll x[maxn], d[maxn];
int n, C;
pair<ll, int> q[maxn];
int ans1, ans2;

bool can(ll diam)
{
	// 	cout << "can " << diam << endl;
	bool needleft = false;
	ll maxsum = inf;
	ll minsum = -inf;
	ll maxdif = inf;
	ll mindif = -inf;
	pair<ll, ll> mostleft = {inf, -inf};
	pair<ll, ll> mostright = {-inf, -inf};
	int l = 0;
	int r = 0;
	for (int i = 0; i < n; i++)
	{
		// no need to check whether it is "small" or not, because in case of "small" its square is inside "big"'s square
		while (r > l && x[i] - q[l].fi + d[i] > diam)
		{
			int wh = q[l].se;
			l++;
			if (x[wh] - d[wh] < mostleft.fi - mostleft.se) mostleft = {x[wh], d[wh]};
			if (x[wh] + d[wh] > mostright.fi + mostright.se) mostright = {x[wh], d[wh]};
			needleft = true;
		}
		if (needleft)
		{
			maxsum = min(maxsum, (mostleft.fi + diam - C - d[i] - mostleft.se) + x[i]);
			minsum = max(minsum, (mostright.fi - (diam - C - d[i] - mostright.se)) + x[i]);
			maxdif = min(maxdif, x[i] - (mostright.fi - (diam - C - d[i] - mostright.se)));
			mindif = max(mindif, x[i] - (mostleft.fi + diam - C - d[i] - mostleft.se));
		}
		while (r > l && q[r - 1].fi > x[i] - d[i]) r--;
		q[r++] = {x[i] - d[i], i};
	}
	// 	cout << minsum << ' ' << maxsum << ' ' << mindif << ' ' << maxdif << endl;
	if (maxsum < minsum || maxdif < mindif) return false;
	return true;
}

long long find_shortcut(int N, vector <int> L0, vector <int> L, int C_)
{
	n = N;
	C = C_;
	ll cursum = 0;
	for (int i = 0; i < n; i++)
	{
		d[i] = L[i];
		x[i] = cursum;
		if (i + 1 < n) cursum += L0[i];
	}
	ll l = 0;
	ll r = inf;
	while (r - l > 1)
	{
		ll mid = (l + r) / 2;
		if (can(mid)) r = mid;
		else l = mid;
	}
	can(r);
	return r;
}

// BEGIN CUT
int main()
{
	int n, c;
	scanf("%d%d", &n, &c);
	
	vector <int> l(n - 1);
	vector <int> d(n);
	for (int i = 0; i < n - 1; i++)
	    scanf("%d", &l[i]);
	for (int i = 0; i < n; i++)
	    scanf("%d", &d[i]);
	    
	long long t = find_shortcut(n, l, d, c);
	
	// BEGIN SECRET
    puts("14e047d7a2907b9034950b074822b302");
    // END SECRET
    
	printf("%lld\n", t);
	
	return 0;
}
// END CUT
