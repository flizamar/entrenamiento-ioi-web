#include<bits/stdc++.h>
#include"tree.h"
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

vector<int> fa;
int find(int u){ return fa[u] == u? u: fa[u] = find(fa[u]);}

int n;
ll sumleaf;
vector< array<ll,2> > ans;

ll query(int l,int r)
{
	int k = r / l;
	ll res;
	if(k > n) res = 0;
	else res = l * ans[k][0] + r * ans[k][1];
	return res + l * sumleaf;
}

void init(vector<int> anc, vector<int> wei)
{
	n = (int)anc.size();
	
	vector< vector<int> > g(n);
	for(int i=1; i<n; ++i)
		g[anc[i]].emplace_back(i);
	
	sumleaf = 0;
	for(int i=0; i<n; ++i) if((int)g[i].size() == 0)
	{
		sumleaf += wei[i];
		wei[i] = 0;
	}
	
	ans.resize(n+2);
	fill(ans.begin(), ans.end(), array<ll,2>{0,0});
	
	auto upd = [&] (int k, int w)
	{
		ans[k-1][0] += (ll)k * w;
		ans[k-1][1] += - w;
	};
	
	vector<pii> eff(n);
	for(int i=0; i<n; ++i)
		eff[i] = {wei[i], i};
	sort(eff.begin(), eff.end());
	reverse(eff.begin(), eff.end());
	
	fa.resize(n);
	iota(fa.begin(), fa.end(), 0);
	
	vector<int> num(n, 1), rig(n, eff[0].first);
	
	auto connect = [&] (int u,int v,int cur)
	{
		u = find(u); v = find(v);
		if(u == v) return;
		upd(num[u], rig[u] - cur);
		upd(num[v], rig[v] - cur);
		num[v] += num[u];
		rig[v] = cur;
		fa[u] = v;
	};
	
	vector<int> val(n, 0);
	
	for(auto t: eff)
	{
		if(t.first == 0) break;
		
		int u = t.second;
		val[u] = 1;
		
		if(anc[u] >= 0 && val[anc[u]])
			connect(u, anc[u], t.first);
		for(int v: g[u])
			connect(v, u, t.first);
		
		int v = find(u);
		if(rig[v] != t.first)
			upd(num[v], rig[v] - t.first);
		--num[v];
		rig[v] = t.first;
	}
	
	for(int i=0; i<n; ++i) if(fa[i] == i)
		upd(num[i], rig[i]);
	
	for(int i=n; i>=1; --i)
	{
		ans[i][0] += ans[i+1][0];
		ans[i][1] += ans[i+1][1];
	}
}

vector<ll> tree(vector<int> anc, vector<int> wei, vector<int> L, vector<int> R)
{
	init(anc, wei);
	
	vector<ll> res(L.size());
	for(int i=0; i<(int)L.size(); ++i)
		res[i] = query(L[i], R[i]);
	return res;
}