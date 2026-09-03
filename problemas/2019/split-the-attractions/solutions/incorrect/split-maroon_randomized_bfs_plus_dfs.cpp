#include "split.h"
#include <bits/stdc++.h>
using namespace std;

using ll=long long;
//#define int ll

#define rng(i,a,b) for(int i=int(a);i<int(b);i++)
#define rep(i,b) rng(i,0,b)
#define gnr(i,a,b) for(int i=int(b)-1;i>=a;i--)
#define per(i,b) gnr(i,0,b)
#define pb push_back
#define eb emplace_back
#define a first
#define b second
#define bg begin()
#define ed end()
#define all(x) x.bg,x.ed
#ifdef LOCAL
#define dmp(x) cerr<<__LINE__<<" "<<#x<<" "<<x<<endl
#else
#define dmp(x) void(0)
#endif

template<class t,class u> void chmax(t&a,u b){if(a<b)a=b;}
template<class t,class u> void chmin(t&a,u b){if(b<a)a=b;}

template<class t> using vc=vector<t>;
template<class t> using vvc=vc<vc<t>>;

using pi=pair<int,int>;
using vi=vc<int>;

template<class t,class u>
ostream& operator<<(ostream& os,const pair<t,u>& p){
	return os<<"{"<<p.a<<","<<p.b<<"}";
}

template<class t> ostream& operator<<(ostream& os,const vc<t>& v){
	os<<"{";
	for(auto e:v)os<<e<<",";
	return os<<"}";
}

ll rand_int(ll l, ll r) { //[l, r]
    static random_device rd;
    static mt19937_64 gen(rd());
    return uniform_int_distribution<ll>(l, r)(gen);
}

const int nmmax=200010;

vc<pi> g[nmmax];
int ans[nmmax];
bool us[nmmax];

void puti(int v,int i,int&rem){
	if(ans[v]||rem==0)return;
	ans[v]=i;
	rem--;
	for(auto e:g[v])if(us[e.b])
		puti(e.a,i,rem);
}

vector<int> find_split(int n, int a, int b, int c, vector<int> p, vector<int> q) {
	vc<pi> si{pi(a,1),pi(b,2),pi(c,3)};
	sort(all(si));
	
	int m=p.size();
	rep(i,m){
		g[p[i]].eb(q[i],i);
		g[q[i]].eb(p[i],i);
	}
	
	//dfs
	{
		vi vis(n),par(n),sz(n,1);
		function<int(int,int)> dfs=[&](int v,int pp){
			if(vis[v])return 0;
			vis[v]=1;
			par[v]=pp;
			for(auto e:g[v]){
				int w=dfs(e.a,v);
				if(w){
					us[e.b]=1;
					sz[v]+=w;
				}
			}
			return sz[v];
		};
		dfs(0,-1);
		rep(i,n){
			rep(k,2){
				if(sz[i]>=si[k].a&&n-sz[i]>=si[k^1].a){
					ans[par[i]]=-1;
					puti(i,si[k].b,si[k].a);
					ans[par[i]]=0;
					puti(par[i],si[k^1].b,si[k^1].a);
					
					assert(si[0].a==0);
					assert(si[1].a==0);
					rep(j,n)if(ans[j]==0)ans[j]=si[2].b;
					return vi(ans,ans+n);
				}
			}
		}
	}
	
	//BFS
	{
		const int B=10;
		
		mt19937 gen;
		
		rep(_,B){
			rep(i,m)us[i]=0;
			
			int r=rand_int(0,n-1);
			vi idx;
			vi vis(n),par(n,-1);
			priority_queue<pi> pq;
			pq.emplace(g[r].size(),r);
			rep(i,n){
				int v=pq.top().b;pq.pop();
				vis[v]=2;
				idx.pb(v);
				vc<pi> es;
				for(auto e:g[v])if(vis[e.a]==2){
					es.pb(e);
				}else if(vis[e.a]==0){
					vis[e.a]=1;
					pq.emplace(g[e.a].size(),e.a);
				}
				if(es.size()){
					pi e=es[rand_int(0,es.size()-1)];
					us[e.b]=1;
					par[v]=e.a;
				}
			}
			
			vi sz(n,1);
			gnr(i,1,n){
				int v=idx[i];
				sz[par[v]]+=sz[v];
			}
			rep(i,n){
				rep(k,2){
					if(sz[i]>=si[k].a&&n-sz[i]>=si[k^1].a){
						ans[par[i]]=-1;
						puti(i,si[k].b,si[k].a);
						ans[par[i]]=0;
						puti(par[i],si[k^1].b,si[k^1].a);
						
						assert(si[0].a==0);
						assert(si[1].a==0);
						rep(j,n)if(ans[j]==0)ans[j]=si[2].b;
						return vi(ans,ans+n);
					}
				}
			}
		}
	}
	
	return vi(n,0);
}
