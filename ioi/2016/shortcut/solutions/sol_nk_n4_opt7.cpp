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
int n, m, C;
ll bestans;
int pos[50];
vector<pair<int, int>> tall;
int k;

// ll checks = 0;
// ll incnt = 0;

ll calc(int a, int b)
{
	if (a > b) swap(a, b);
// 	checks++;
	ll curd = 0;
	for (int i = 0; i < k; i++)
	{
		for (int j = i + 1; j < k; j++)
		{
			curd = max(curd, d[pos[i]] + d[pos[j]] + min(abs(x[pos[j]] - x[pos[i]]), C + min(abs(x[pos[i]] - x[a]) + abs(x[pos[j]] - x[b]), abs(x[pos[j]] - x[a]) + abs(x[pos[i]] - x[b]))));
		}
	}
	if (curd > bestans) return curd;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			curd = max(curd, min(d[i] + d[j] + x[j] - x[i], d[i] + d[j] + abs(x[i] - x[a]) + abs(x[j] - x[b]) + C));
// 			incnt++;
			if (curd > bestans) return curd;
		}
	}
	return curd;
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
	for (int i = 0; i < n; i++) tall.pb({L[i], i});
	sort(all(tall));
	reverse(all(tall));
	long long result = inf;
	bestans = inf;
	int end1 = 0;
	int end2 = 1;
	k = min(n, 10);
	for (int i = 0; i < k; i++)
	{
		for (int j = i + 1; j < k; j++)
		{
			ll curans = calc(tall[i].se, tall[j].se);
			if (curans < bestans)
			{
				bestans = curans;
				result = curans;
				end1 = tall[i].se;
				end2 = tall[j].se;
			}
		}
	}
	for (int i = 0; i < k; i++) pos[i] = tall[i].se;
// 	cerr << bestans << endl;
	for (int i = 0; i < n; i++)
	{
		ll shouldmove = 0;
		for (int j = i + 1; j < n; j++)
		{
			shouldmove -= L0[j - 1];
			if (shouldmove <= 0)
			{
				ll curans = calc(i, j);
				if (curans < bestans)
				{
					bestans = curans;
					result = curans;
					end1 = i;
					end2 = j;
				} else
				{
					shouldmove = curans - bestans;
				}
			}
		}
	}
	
	return result;
	// 	cerr << checks << ' ' << incnt << endl;
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
