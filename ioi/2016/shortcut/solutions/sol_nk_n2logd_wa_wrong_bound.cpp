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
int ans1, ans2;
int n, m;
int C;

inline bool in(ll x, ll lb, ll ub)
{
	return x >= lb && x <= ub;
}

bool can(ll maxd)
{
	ll maxsum = inf;
	ll minsum = -inf;
	ll maxdif = inf;
	ll mindif = -inf;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (x[j] - x[i] + d[j] + d[i] > maxd)
			{
				maxsum = min(maxsum, x[i] + x[j] + (maxd - C - d[i] - d[j]));
				minsum = max(minsum, x[i] + x[j] - (maxd - C  - d[i] - d[j]));
				maxdif = min(maxdif, x[j] - x[i] + (maxd - C  - d[i] - d[j]));
				mindif = max(mindif, x[j] - x[i] - (maxd - C  - d[i] - d[j]));
			}
		}
	}
	if (maxsum < minsum || maxdif < mindif) return false;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++) if (in(x[i] + x[j], minsum, maxsum) && in(x[j] - x[i], mindif, maxdif))
		{
			ans1 = i;
			ans2 = j;
			return true;
		}
	}
	return false;
}

long long find_shortcut(int N, vector <int> L0, vector <int> L, int C_)
{
	C = C_;
	n = N;
	ll curlen = 0;
	for (int i = 0; i < n; i++)
	{
		d[i] = L[i];
		x[i] = curlen;
		if (i + 1 < n) curlen += L0[i];
	}
	ans1 = 0;
	ans2 = 1;
	ll l = 0;
	ll r = curlen;
	while (r - l > 1)
	{
		ll mid = (l + r) / 2;
		if (can(mid)) r = mid;
		else l = mid;
	}
// 	cerr << r << ' ' << curlen << endl;
// 	cerr << minres << endl;
// 	for (int i = 0; i < n; i++) cerr << d[i] << ' ';
// 	cerr << endl;
	can(r);
	return r;
//	end1 = ans1;
//	end2 = ans2;
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
}// END CUT
