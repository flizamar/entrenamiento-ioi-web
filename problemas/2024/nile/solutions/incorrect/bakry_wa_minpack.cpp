#include <bits/stdc++.h>

using namespace std ;

const int MAX = 1e5 + 10 ;

int par[MAX], sz[MAX] ;
int n, q;
int ans = 0 ;

void init()
{
	for(int i = 0; i < n; ++i)
		par[i] = i, sz[i] = 1;
}

int root(int node)
{
	if(par[node] == node)
		return node ;
	return (par[node] = root(par[node])) ;
}

int get(int x)
{
	if(x%2 == 0)
		return x ;
	else
		return (x-1) + 2;
}

void Union(int x , int y)
{
	int a = root(x) ;
	int b = root(y) ;
	if(a == b)
		return ;
	if(sz[a] < sz[b])
		swap(a , b); 
	ans -= get(sz[a]);
	ans -= get(sz[b]);
	par[b] = a ;
	sz[a] += sz[b];
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
		v.push_back({W[i+1] - W[i], -1, i}) ;
	for(int i = 0 ; i < q ; ++i)
		v.push_back({E[i], 1, i});
	sort(v.begin(), v.end()) ;
	init();
	vector<long long>vans(q);
	ans = 2*n ;
	for(auto &a : v)
	{
		if(a[1] == -1)
			Union(a[2], a[2]+1);
		else
			vans[a[2]] = ans;
	}
	return vans;
}