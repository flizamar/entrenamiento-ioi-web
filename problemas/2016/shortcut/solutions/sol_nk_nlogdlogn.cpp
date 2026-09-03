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

const int maxn = 3000005;
const ll inf = 1e18;

ll x[maxn], d[maxn];
int n, C;
priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> q;
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
	while (!q.empty()) q.pop();
	for (int i = 0; i < n; i++)
	{
		// no need to check whether it is "small" or not, because in case of "small" its square is inside "big"'s square
		while (!q.empty() && x[i] - q.top().fi + d[i] > diam)
		{
			int wh = q.top().se;
			q.pop();
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
		q.push({x[i] - d[i], i});
	}
// 	cout << minsum << ' ' << maxsum << ' ' << mindif << ' ' << maxdif << endl;
	if (maxsum < minsum || maxdif < mindif) return false;
	int curdif = 0;
	int cursum = n;
	for (int i = 0; i < n; i++)
	{
		while (curdif < n && x[curdif] - x[i] < mindif) curdif++;
		while (cursum > 0 && x[cursum - 1] + x[i] >= minsum) cursum--;
		int cur = max({cursum, curdif, i + 1});
		if (cur < n && x[cur] + x[i] <= maxsum && x[cur] - x[i] <= maxdif)
		{
			ans1 = i;
			ans2 = cur;
			return true;
		}
	}
	return false;
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
	long long result = inf;
	int end1 = 0;
	int end2 = 1;
	ll l = 0;
	ll r = inf;
	while (r - l > 1)
	{
		ll mid = (l + r) / 2;
		if (can(mid)) r = mid;
		else l = mid;
	}
	can(r);
	end1 = ans1;
	end2 = ans2;
	result = r;
	
	return result;
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
