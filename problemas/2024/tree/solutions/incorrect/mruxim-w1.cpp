#include<bits/stdc++.h>
using namespace std;

#define rep(i, n) for(int i = 0, i##__n = (int)(n); i < i##__n; ++i)
#define fer(i, a, b) for(int i = (int)(a), i##__b = (int)(b); i < i##__b; ++i)
#define rof(i, b, a) for(int i = (int)(b), i##__a = (int)(a); i-- > i##__a; )
#define sz(x) (int((x).size()))
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define X first
#define Y second
//#define endl '\n'

template<class P, class Q> inline void smin(P &a, Q b) { if (b < a) a = b; }
template<class P, class Q> inline void smax(P &a, Q b) { if (a < b) a = b; }

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;

////////////////////////////////////////////////////////////////////////////////

const int maxn = 300'000 + 100;

int n;
vector<int> p, w;
int cnt[maxn];
int leaves_cnt;

void init(vector<int> _p, vector<int> _w) {
	p = _p, w = _w;
	n = sz(p);
	
	fer(i, 1, n) cnt[p[i]]++;
	leaves_cnt = 0;
	rep(i, n)
		leaves_cnt += (cnt[i] == 0 ? 1 : 0);
}

ll query(int l, int r) {
	return leaves_cnt * (ll)l + max(0ll, leaves_cnt * (ll)l - r);
}
