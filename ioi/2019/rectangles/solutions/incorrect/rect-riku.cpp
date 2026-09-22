#include "rect.h"
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

struct BIT{
	vi buf;
	int s;
	BIT(int n=0){init(n);}
	void init(int n){buf.assign(s=n,0);}
	void add(int i,int v){
		for(;i<s;i+=(i+1)&(-i-1))
			buf[i]+=v;
	}
	int get(int i){
		int res=0;
		for(;i>=0;i-=(i+1)&(-i-1))
			res+=buf[i];
		return res;
	}
	int sum(int b,int e){
		return get(e-1)-get(b-1);
	}
};

vc<pi> getrng(vi a){
	int n=a.size();
	vc<pi> res;
	vi cur;
	rep(i,n){
		int s;
		while((s=cur.size())&&a[cur[s-1]]<a[i]){
			if(s>=2&&a[cur[s-2]]>a[cur[s-1]])
				res.eb(cur[s-2]+1,i);
			cur.pop_back();
		}
		cur.pb(i);
	}
	return res;
}

ll count_rectangles(vvc<int> H){
	dmp(H);
	int n=H.size(),m=H[0].size();
	vvc<pi> vs(n);
	
	rep(i,m){
		vi a(n);
		rep(j,n)a[j]=H[j][i];
		auto w=getrng(a);
		for(auto r:w)
			vs[r.b-1].eb(r.b-r.a,i);
	}
	
	ll ans=0;
	
	vvc<int> cur(m+1,vi(m+1)),nx(m+1,vi(m+1));
	vc<pi> prr;
	BIT bit(m*(m+1));
	rep(i,n){
		using Q=tuple<int,int,int>;
		vc<Q> qs;
		{
			auto w=getrng(H[i]);
			for(auto p:w){
				int h=cur[p.a][p.b]+1;
				nx[p.a][p.b]=h;
				qs.eb(h,p.a,p.b);
			}
			for(auto p:prr)
				cur[p.a][p.b]=0;
			swap(cur,nx);
			swap(prr,w);
		}
		for(auto p:vs[i])
			qs.eb(p.a,-1,p.b);
		sort(all(qs),greater<Q>());
		vi pos;
		int lsh=-1,lsl=-1,lsr=-1;
		for(auto q:qs){
			int h,x,y;tie(h,x,y)=q;
			//cerr<<i<<" "<<h<<" "<<x<<" "<<y<<endl;
			if(x==-1){
				if(lsh==h&&y==lsl-1){
					lsl=y;
				}else{
					lsh=h;
					lsl=lsr=y;
				}
				ans+=bit.sum(lsl*(m+1),lsl*(m+1)+lsr+2);
			}else{
				bit.add(x*(m+1)+y,1);
				pos.pb(x*(m+1)+y);
			}
		}
		for(auto j:pos)
			bit.add(j,-1);
		//cerr<<i<<" "<<ans<<endl;
	}
	
	return ans;
}
