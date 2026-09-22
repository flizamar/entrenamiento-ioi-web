// Complexity: O(qnlog^2 n)
// One can change the 'map' in 'DS' into other DS to obtain a complexity of O(qnlog n)
#include<bits/stdc++.h>
#include"tree.h"
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const int MAXN = 3e5 + 5;

struct DS
{
	map<int,ll> q;
	void clear(void){ q.clear();}
	void insert(int x,ll y){ q[x] += y;}
	ll cutl(ll k)
	{
		ll res = 0;
		while(q.size() && q.begin() -> second <= k)
		{
			auto t = *q.begin();
			k -= t.second;
			res += (ll)t.first * t.second;
			q.erase(q.begin());
		}
		if(q.size() && k)
		{
			q.begin() -> second -= k;
			res += (ll)q.begin() -> first * k;
		}
		return res;
	}
	void cutr(ll k)
	{
		while(q.size() && q.rbegin() -> second <= k)
		{
			k -= q.rbegin() -> second;
			q.erase(prev(q.end()));
		}
		if(q.size() && k)
		{
			q.rbegin() -> second -= k;
		}
	}
	void merge(DS &oth)
	{
		if(q.size() < oth.q.size())
			q.swap(oth.q);
		for(auto t: oth.q)
			q[t.first] += t.second;
		oth.q.clear();
	}
	void replace(int k)
	{
		ll len = 0;
		while(q.size() && q.rbegin() -> first > k)
		{
			len += q.rbegin() -> second;
			q.erase(prev(q.end()));
		}
		q[k] += len;
	}
};

struct Hull
{
	ll c, val, l, r;
	DS hl, hr;
	void init(void)
	{
		c = l = r = 0; val = 0;
		hl.clear(); hr.clear();
	}
	void merge(Hull &oth)
	{
		c += oth.c; val += oth.val;
		l += oth.l; r += oth.r;
		hl.merge(oth.hl);
		hr.merge(oth.hr);
	}
	void trim(int ql,int qr,int w)
	{
		if(r < qr) hr.insert(w, qr - r), r = qr;
		if(l > ql) hl.insert(w, l - ql), l = ql;
		
		hl.replace(w);
		hr.replace(w);
		
		if(c < ql)
		{
			hl.clear(); val += hr.cutl(ql - c); c = l = ql;
		}
		if(c > qr)
		{
			hr.clear(); val += hl.cutl(c - qr); c = r = qr;
		}
		
		if(r > qr) hr.cutr(r - qr), r = qr;
		if(l < ql) hl.cutr(ql - l), l = ql;
	}
	void print(void)
	{
		printf("c = %lld, val = %lld, l = %lld, r = %lld\n",c,val,l,r);
		printf("hl: ");
		for(auto t: hl.q)
			printf("(%d, %lld), ",t.first,t.second);
		printf("\n");
		printf("hr: ");
		for(auto t: hr.q)
			printf("(%d, %lld), ",t.first,t.second);
		printf("\n");
	}
};

int n;
vector<int> g[MAXN];
int anc[MAXN], wei[MAXN];

Hull f[MAXN];

ll query(int ql,int qr)
{
//	printf("query: ql = %d, qr = %d\n",ql,qr);
	
	for(int u=n; u>=1; --u)
	{
		f[u].init();
		for(int v: g[u])
			f[u].merge(f[v]);
		f[u].trim(ql, qr, wei[u]);
		
//		printf("g[%d]: ",u);
//		for(auto v: g[u])
//			printf("%d ",v);
//		printf("\n");
//		
//		printf("f[%d]:\n",u);
//		f[u].print();
//		printf("\n");
	}
	return f[1].val;
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
}