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

multiset<ll> leftpart, rightpart;
ll x[maxn], d[maxn];
ll l0[maxn], L[maxn];
int n, m, C;
ll bestans;

ll calc(int a, int b)
{
	if (a > b) swap(a, b);
	
	ll dleft = 0;
	ll mind = 0;
	ll curx = 0;
	for (int i = 0; i <= a; i++)
	{
		mind = max(mind, L[i] + curx - dleft);
		dleft = min(dleft, curx - L[i]);
		if (i < a) curx += l0[i];
	}
	d[0] = curx - dleft;
	x[0] = curx;
	curx += l0[a];
	
	for (int i = a + 1; i < b; i++)
	{
		x[i - a] = curx;
		curx += l0[i];
		d[i - a] = L[i];
	}
	for (int i = b; i < n - 1; i++) curx += l0[i];
	
	ll dright = curx;
	for (int i = n - 1; i >= b; i--)
	{
		mind = max(mind, L[i] + dright - curx);
		dright = max(dright, curx + L[i]);
		if (i > b) curx -= l0[i - 1];
	}
	x[b - a] = curx;
	d[b - a] = dright - curx;
	for (int i = 1; i <= b - a; i++) x[i] -= x[0];
	x[0] = 0;
	
	int nn = b - a + 1;
	
	long long curd = 0;
	
	ll totallen = x[nn - 1] + C;
	for (int i = 0; i < nn; i++)
	{
		x[i + nn] = x[i] + totallen;
		d[i + nn] = d[i];
	}
	for (int i = nn; i < 2 * nn; i++)
	{
		x[i + nn] = x[i] + totallen;
		d[i + nn] = d[i];
	}
	leftpart.clear();
	rightpart.clear();
	int curp = nn + 1;
	rightpart.insert(x[nn] + d[nn]);
	for (int i = nn + 1; i < 2 * nn; i++)
	{
		if (x[i] - x[nn] < totallen - (x[i] - x[nn]))
		{
			curp = i + 1;
			rightpart.insert(x[i] + d[i]);
		} else
		{
			break;
		}
	}
	for (int i = 2 * nn - 1; i >= curp; i--) leftpart.insert(-x[i - nn] + d[i - nn]);
	for (int i = nn; i < 2 * nn; i++)
	{
		while (curp < i + nn && x[curp] - x[i] < totallen - (x[curp] - x[i]))
		{
			leftpart.erase(leftpart.find(-x[curp - nn] + d[curp - nn]));
			rightpart.insert(x[curp] + d[curp]);
			curp++;
		}
		rightpart.erase(rightpart.find(x[i] + d[i]));
		if (!leftpart.empty()) curd = max(curd, x[i] + *leftpart.rbegin() + d[i]);
		if (!rightpart.empty()) curd = max(curd, *rightpart.rbegin() - x[i] + d[i]);
		leftpart.insert(-x[i] + d[i]);
	}
	
	curd = max(curd, mind);

	return curd;
}

long long find_shortcut(int N, vector <int> L0, vector <int> L_, int C_)
{
	n = N;
	C = C_;
	ll cursum = 0;
	for (int i = 0; i < n; i++)
	{
		L[i] = L_[i];
		if (i + 1 < n) {
		    l0[i] = L0[i];
		    cursum += L0[i];
		}
	}
	long long result = inf;
	bestans = inf;
	int end1 = 0;
	int end2 = 1;
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
