#include "nile.h"
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;


#define rep(i, a, b) for(int i = (a); i < (b); ++ i)
#define SZ(x) ((int)(x).size())
#define all(x) (x).begin(), (x).end()
#define F first
#define S second
#define pb push_back

const int maxn = 2e5 + 100;
const int inf = 1e9 + 100;

ll cost = 0;

int dsu[maxn];
int sz[maxn];
int mn[maxn][3];

int a[maxn];
int c[maxn];

int root(int v){
    if(dsu[v] == v) return v;
    return dsu[v] = root(dsu[v]);
}

inline int Calc(int v){
    assert(v == root(v));
    return ((sz[v] & 1) ? min(mn[v][2], mn[v][v & 1]) : 0);
}

void Vol(int i){
    assert(root(i) == root(i + 1) && root(i) == root(i - 1));
    int v = root(i);
    cost -= Calc(v);
    mn[v][2] = min(mn[v][2], c[i]);
    cost += Calc(v);
}

void Union(int v, int u){
    v = root(v);
    u = root(u);
    assert(v != u);
    if(v > u) swap(v, u);
    cost -= Calc(v);
    cost -= Calc(u);
    dsu[u] = v;
    sz[v] += sz[u];
    rep(i, 0, 3){
        mn[v][i] = min(mn[v][i], mn[u][i]);
    }
    cost += Calc(v);
}

vector<long long>calculate_costs(vector<int>W, vector<int>A, vector<int>B, vector<int>E) {
    vector<ll> ans(SZ(E));
	int n = SZ(W);
	ll s = 0;
	for(int i : B) s += i;
	vector<pii> vec(n);
	rep(i, 0, n){
		vec[i] = {W[i], A[i] - B[i]};
	}
	sort(all(vec));
	
	rep(i, 0, n){
		a[i] = vec[i].F;
		c[i] = vec[i].S;
        assert(c[i] < inf);
        assert(c[i] >= 0);
	}

    rep(i, 0, n){
        dsu[i] = i;
        sz[i] = 1;
        rep(j, 0, 3) mn[i][j] = inf;
        mn[i][i & 1] = c[i];
        cost += Calc(i);
    }

    vector<pair<int, pii> > sonic;

    rep(i, 1, n){
        sonic.pb({a[i] - a[i - 1], {0, i}});
    }
    rep(i, 1, n - 1){
        sonic.pb({a[i + 1] - a[i - 1], {1, i}});
    }
    rep(i, 0, SZ(E)){
        sonic.pb({E[i], {2, i}});
    }
    
    sort(all(sonic));

    for(auto x : sonic){
        int t = x.S.F;
        int i = x.S.S;
        if(t == 0){
            Union(i, i - 1);
        }
        else if(t == 1){
            Vol(i);
        }
        else{
            ans[i] = s + cost;
        }
    }
	
    return ans;
}