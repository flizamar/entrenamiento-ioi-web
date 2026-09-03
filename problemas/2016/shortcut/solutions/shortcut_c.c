#include <stdio.h>
#include <stdlib.h>

typedef long long ll;
typedef struct llpair {
	ll fi;
	ll se;
} llpair;

#define maxn 1000005
const ll inf = 1e18;

ll x[maxn], d[maxn];
int n, C;
llpair q[maxn];
int ans1, ans2;

llpair mp(ll fi, ll se) {
	llpair res;
	res.fi = fi;
	res.se = se;
	return res;
}

ll min(ll a, ll b) {
	return a < b ? a : b;
}

ll max(ll a, ll b) {
	return a > b ? a : b;
}

ll max3(ll a, ll b, ll c) {
	return max(max(a, b), c);
}

int can(ll diam)
{
// 	cout << "can " << diam << endl;
	int needleft = 0;
	ll maxsum = inf;
	ll minsum = -inf;
	ll maxdif = inf;
	ll mindif = -inf;
	llpair mostleft;
	mostleft.fi = inf;
	mostleft.se = -inf;
	llpair mostright;
	mostright.fi = -inf;
	mostright.se = -inf;
	int l = 0;
	int r = 0;
	for (int i = 0; i < n; i++)
	{
		// no need to check whether it is "small" or not, because in case of "small" its square is inside "big"'s square
		while (r > l && x[i] - q[l].fi + d[i] > diam)
		{
			int wh = q[l].se;
			l++;
			if (x[wh] - d[wh] < mostleft.fi - mostleft.se) mostleft = mp(x[wh], d[wh]);
			if (x[wh] + d[wh] > mostright.fi + mostright.se) mostright = mp(x[wh], d[wh]);
			needleft = 1;
		}
		if (needleft)
		{
			maxsum = min(maxsum, (mostleft.fi + diam - C - d[i] - mostleft.se) + x[i]);
			minsum = max(minsum, (mostright.fi - (diam - C - d[i] - mostright.se)) + x[i]);
			maxdif = min(maxdif, x[i] - (mostright.fi - (diam - C - d[i] - mostright.se)));
			mindif = max(mindif, x[i] - (mostleft.fi + diam - C - d[i] - mostleft.se));
		}
		while (r > l && q[r - 1].fi > x[i] - d[i]) r--;
		q[r++] = mp(x[i] - d[i], i);
	}
// 	cout << minsum << ' ' << maxsum << ' ' << mindif << ' ' << maxdif << endl;
	if (maxsum < minsum || maxdif < mindif) return 0;
	int curdif = 0;
	int cursum = n;
	for (int i = 0; i < n; i++)
	{
		while (curdif < n && x[curdif] - x[i] < mindif) curdif++;
		while (cursum > 0 && x[cursum - 1] + x[i] >= minsum) cursum--;
		int cur = max3(cursum, curdif, i + 1);
		if (cur < n && x[cur] + x[i] <= maxsum && x[cur] - x[i] <= maxdif)
		{
			ans1 = i;
			ans2 = cur;
			return 1;
		}
	}
	return 0;
}

long long find_shortcut(int N, int* L0, int* L, int C_)
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
	int *l = (int *)malloc((n - 1) * sizeof(int)); // N > 1
	int *d = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < n - 1; i++) {
	    scanf("%d", &l[i]);
	}
	for (int i = 0; i < n; i++) {
	    scanf("%d", &d[i]);
	}
	long long t = find_shortcut(n, l, d, c);
	
    // BEGIN SECRET
    puts("14e047d7a2907b9034950b074822b302");
    // END SECRET
	
	printf("%lld\n", t);
	
	return 0;
}

// END CUT
