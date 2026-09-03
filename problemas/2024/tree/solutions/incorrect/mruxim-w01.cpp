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
int leaves_cnt[maxn];
int leaves_sum;
vector<int> cnts;
vector<int> psum;

void init(vector<int> _p, vector<int> _w) {
	p = _p, w = _w;
	n = sz(p);
	
	fer(i, 1, n) cnt[p[i]]++;

	leaves_sum = 0;
	rof(i, n, 0) {
		leaves_sum += (cnt[i] == 0 ? w[i] : 0);
		leaves_cnt[i] += (cnt[i] == 0 || w[i] == 0 ? 1 : 0);
		if(i > 0 && w[p[i]] == 1)
			leaves_cnt[p[i]] += leaves_cnt[i];
		else if(w[i] == 1)
			cnts.pb(leaves_cnt[i]);
	}

	sort(all(cnts));

	psum.assign(sz(cnts) + 1, 0);
	partial_sum(all(cnts), psum.begin() + 1);
	
}

ll query(int l, int r) {
	int pos = upper_bound(all(cnts), r / l) - cnts.begin();
	return leaves_sum * (ll)l + (psum.back() - psum[pos]) * (ll)l - (sz(psum) - 1 - pos) * (ll)r;
}
