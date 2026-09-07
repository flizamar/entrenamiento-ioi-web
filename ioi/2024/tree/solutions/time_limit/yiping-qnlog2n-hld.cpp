// Complexity: O(qnlog^2 n)
// One can change the 'map' in 'DS' into other DS to obtain a complexity of O(qnlog n)
#include<bits/stdc++.h>
#include"tree.h"
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const int MAXN = 3e5 + 5;
const int inf = 0x3f3f3f3f;

struct Segment_Tree
{
	int mn[MAXN<<2], tag[MAXN<<2], n;
	#define ls(u) ((u)<<1)
	#define rs(u) ((u)<<1|1)
	#define lson(u) ls(u),l,mid
	#define rson(u) rs(u),mid+1,r
	inline void push_up(int u)
	{
		mn[u] = min(mn[ls(u)], mn[rs(u)]) + tag[u];
	}
	void build(int u,int l,int r)
	{
		mn[u] = 0; tag[u] = 0;
		if(l == r) return;
		int mid = (l+r)>>1;
		build(lson(u)); build(rson(u));
	}
	void add(int u,int l,int r,int ql,int qr,int k)
	{
		if(ql<=l && r<=qr){ tag[u] += k; mn[u] += k; return;}
		int mid = (l+r)>>1;
		if(ql<=mid) add(lson(u),ql,qr,k);
		if(mid<qr) add(rson(u),ql,qr,k);
		push_up(u);
	}
	int query(int u,int l,int r,int ql,int qr)
	{
		if(ql<=l && r<=qr) return mn[u];
		int mid = (l+r)>>1;
		if(ql<=mid && mid<qr) return min(query(lson(u),ql,qr), query(rson(u),ql,qr)) + tag[u];
		else return (ql<=mid? query(lson(u),ql,qr): query(rson(u),ql,qr)) + tag[u];
	}
	void init(int _n){ n = _n; build(1,1,n);}
	void add(int ql,int qr,int k){ if(ql<=qr) add(1,1,n,ql,qr,k);}
	int query(int ql,int qr){ return ql<=qr? query(1,1,n,ql,qr): inf;}
}tree;

int n;
vector<int> g[MAXN];
int anc[MAXN], wei[MAXN];

int siz[MAXN], dep[MAXN], son[MAXN];
void dfs_tree(int u,int fa)
{
	siz[u] = 1; son[u] = 0;
	for(int v: g[u]) if(v != fa)
	{
		dep[v] = dep[u] + 1;
		dfs_tree(v,u);
		siz[u] += siz[v];
		if(siz[v] > siz[son[u]]) son[u] = v;
	}
}

int dfn[MAXN], seq[MAXN], top[MAXN], curdfn;
void dfs_dfn(int u,int fa,int tp)
{
	top[u] = tp;
	dfn[u] = ++curdfn; seq[curdfn] = u;
	if(son[u]) dfs_dfn(son[u], u, tp);
	for(int v: g[u]) if(v != fa && v != son[u])
		dfs_dfn(v, u, v);
}

int getmin_path(int u,int v)// v is anc of u
{
	int res = inf;
	while(dep[top[u]] > dep[v])
		res = min(res, tree.query(dfn[top[u]], dfn[u])), u = anc[top[u]];
	res = min(res, tree.query(dfn[v] + 1, dfn[u]));
	return res;
}
void add_path(int u,int v,int k)// v is anc of u
{
	while(dep[top[u]] > dep[v])
		tree.add(dfn[top[u]], dfn[u], k), u = anc[top[u]];
	tree.add(dfn[v] + 1, dfn[u], k);
}

ll sum[MAXN];
set<pii> h[MAXN];

ll query(int ql,int qr)
{
	ll ans = 0;
	tree.init(n);
	for(int u=n; u>=1; --u)
	{
		h[u].clear();
		if(g[u].size() == 0)
		{
			ans += (ll)ql * wei[u];
			sum[u] = ql;
			h[u].emplace(wei[u], u);
			continue;
		}
		
		sum[u] = 0;
		h[u].emplace(wei[u], u);
		for(int v: g[u])
		{
			sum[u] += sum[v];
			if(h[u].size() < h[v].size())
				h[u].swap(h[v]);
			for(auto t: h[v])
				h[u].insert(t);
			h[v].clear();
		}
		
		while(sum[u] > qr)
		{
			auto v = h[u].begin() -> second;
			if(v == u)
			{
				ans += (sum[u] - qr) * wei[u];
				sum[u] = qr;
				break;
			}
			
			ll t = getmin_path(v, u);
			ll k = min(t, sum[u] - qr);
			
			if(k == 0)
			{
				h[u].erase(h[u].begin());
				continue;
			}
			
			ans += k * wei[v];
			add_path(v, u, -k);
			sum[u] -= k;
			
			if(t == k)
			{
				h[u].erase(h[u].begin());
			}
		}
		
		tree.add(dfn[u], dfn[u], sum[u] - ql);
	}
	
	return ans;
}

void init(vector<int> _anc, vector<int> _wei)
{
	n = (int)_anc.size();
	for(int i=1; i<=n; ++i)
		g[i].clear();
	for(int i=1; i<=n; ++i)
	{
		anc[i] = _anc[i-1] + 1;
		wei[i] = _wei[i-1];
		if(i > 1)
			g[anc[i]].emplace_back(i);
	}
	
	curdfn = 0; dep[1] = 0;
	dfs_tree(1,0);
	dfs_dfn(1,0,1);
}