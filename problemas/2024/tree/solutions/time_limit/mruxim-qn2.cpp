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
vector<int> ch[maxn];
ll sum[maxn];
set<pii> st[maxn];

void init(vector<int> _p, vector<int> _w) {
	p = _p, w = _w;
	n = sz(p);

	fer(i, 1, n) ch[p[i]].pb(i);
}

void dfs_rem(int u, set<pii> &ss) {
	if(!ss.erase(pii(w[u], u))) return;
	for(int v: ch[u]) dfs_rem(v, ss);
}

ll query(int l, int r) {
	fill(sum, sum+n, 0);
	rep(i, n) st[i].clear();

	ll ans = 0;
	rof(i, n, 0) {
		if(sz(ch[i]) == 0) {
			sum[i] = l;
			ans += l * (ll)w[i];
		} else {
			int u = i;
			if(sum[u] - l > 0) st[u].insert({w[u], u});
			while(r < sum[u]) {
				int v = (*st[u].begin()).second;
				ll d = sum[u] - r;
				int g = -1;
				for(int x = v; x != p[u]; x = p[x]) smin(d, sum[x] - l), (d == sum[x] - l && (g = x));
				for(int x = v; x != p[u]; x = p[x]) sum[x] -= d;
				ans += d * (ll)w[v];
				if(g != -1) dfs_rem(g, st[u]);
			}
		}

		if(i) {
			sum[p[i]] += sum[i];
			if(sz(st[p[i]]) < sz(st[i])) st[p[i]].swap(st[i]);
			st[p[i]].insert(all(st[i]));
		}
	}

	return ans;
}
