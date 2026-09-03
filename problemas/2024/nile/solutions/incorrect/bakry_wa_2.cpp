#include <bits/stdc++.h>

using namespace std ;

const int MAX = 1e5 + 10 ;

int par[MAX], sz[MAX], W[MAX], R[MAX];
int n, q;

long long sum[MAX];
long long val[MAX];

multiset<long long>s[MAX];

long long ans = 0 ;

void init()
{
	for(int i = 0; i < n; ++i)
		par[i] = i, sz[i] = 1, W[i] = R[i] = i, s[i].clear(), s[i].insert(1e18);
}

int root(int node)
{
	if(par[node] == node)
		return node ;
	return (par[node] = root(par[node])) ;
}

long long get(int x)
{
	if(sz[x] % 2 == 0)
		return sum[x];
	else
		return sum[x] + min({(*s[x].begin()), val[W[x]], val[R[x]]});
}

void Union(int x , int y)
{
	int a = root(x) ;
	int b = root(y) ;
	if(a == b)
		return ;
	if(sz[a] < sz[b])
		swap(a , b); 
	ans -= get(a);
	ans -= get(b);
	par[b] = a ;
	sz[a] += sz[b];
	while(s[b].size())
		s[a].insert(*s[b].begin()), s[b].erase(s[b].begin());
	W[a] = min(W[a], W[b]);
	R[a] = max(R[a], R[b]);
	ans += get(sz[a]);
}

vector<long long>calculate_costs(vector<int>W, vector<int>A, vector<int>B, vector<int>E) {
	vector< array<int , 3> >ord;
	int n = W.size(), q = E.size();
	for(int i = 0; i < n; ++i)
		ord.push_back({W[i], A[i], B[i]});
	sort(ord.begin(), ord.end());
	for(int i = 0; i < n; ++i)
		W[i] = ord[i][0], A[i] = ord[i][1], B[i] = ord[i][2];
	vector< array<int , 3> >v ;
	for(int i = 0 ; i < n-1 ; ++i)
	{
		v.push_back({W[i+1] - W[i], -1, i});
		if(i+2 < n)
			v.push_back({W[i+2] - W[i], -2, i+1});
	}
	for(int i = 0 ; i < q ; ++i)
		v.push_back({E[i], 1, i});
	sort(v.begin(), v.end()) ;
	init();
	ans = 0 ;
	for(int i = 0 ; i < n; ++i)
		sum[i] += B[i], val[i] = A[i] - B[i], ans += A[i];
	vector<long long>vans(q);
	for(auto &a : v)
	{
		if(a[1] == -1)
			Union(a[2], a[2]+1);
		else if(a[1] == -2)
		{
			int x = root(a[2]);
			ans -= get(x);
			s[x].insert(val[a[2]]);
			ans += get(x);
		}
		else
			vans[a[2]] = ans;
	}
	return vans;
}