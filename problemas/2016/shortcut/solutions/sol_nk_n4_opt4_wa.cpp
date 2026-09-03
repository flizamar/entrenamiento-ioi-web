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

ll checks = 0;
ll incnt = 0;

ll calc(int a, int b)
{
	checks++;
	ll curd = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			curd = max(curd, min(d[i] + d[j] + x[j] - x[i], d[i] + d[j] + abs(x[i] - x[a]) + abs(x[j] - x[b]) + C));
			incnt++;
// 			if (curd > bestans) return curd;
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
	bestans = inf;
	int end1 = 0;
	int end2 = 1;
	ll shouldmove = 0;
	for (int i = 0; i < n; i++)
	{
		if (i > 0) shouldmove -= L0[i - 1];
		if (shouldmove <= 0)
		{
			ll bestfori = inf;
			ll shouldmovej = 0;
			for (int j = i + 1; j < n; j++)
			{
				shouldmovej -= L0[j - 1];
				if (shouldmovej <= 0)
				{
					ll curans = calc(i, j);
					bestfori = min(bestfori, curans);
					if (curans < bestans)
					{
						bestans = curans;
						result = curans;
						end1 = i;
						end2 = j;
					}
					shouldmovej = curans - bestans;
				}
			}
			shouldmove = bestfori - bestans;
		}
	}
	
	return result;
	//cerr << checks << ' ' << incnt << endl;
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
