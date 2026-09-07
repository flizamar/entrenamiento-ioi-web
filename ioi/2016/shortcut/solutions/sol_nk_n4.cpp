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

ll calc(int a, int b)
{
	ll curd = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			curd = max(curd, min(d[i] + d[j] + x[j] - x[i], d[i] + d[j] + abs(x[i] - x[a]) + abs(x[j] - x[b]) + C));
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
	long long result = inf;
	int end1 = 0;
	int end2 = 1;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			ll curans = calc(i, j);
			if (curans < result)
			{
				result = curans;
				end1 = i;
				end2 = j;
			}
		}
	}
	
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
