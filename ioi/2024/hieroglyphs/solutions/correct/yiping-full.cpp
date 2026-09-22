#include<bits/stdc++.h>
#include"hieroglyphs.h"
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const int inf = 0x3f3f3f3f;

struct Data
{
	int d;
	vector<int> a, cnt, rnk;
	vector< vector<int> > pos;
	
	Data(vector<int> _a, int _d)
	{
		a = _a; d = _d;
		cnt.resize(d); pos.resize(d); rnk.resize(a.size());
		for(int i=0; i<(int)a.size(); ++i)
		{
			int x = a[i];
			rnk[i] = cnt[x]; ++cnt[x];
			pos[x].emplace_back(i);
		}
	}
	bool check(int x,int cx,int y,int cy) const
	{
		if(cx > (int)pos[x].size() || cy > (int)pos[y].size())
			return 0;
		if(cx == 0 || cy == 0) return 1;
		return pos[x][cx - 1] < pos[y][(int)pos[y].size() - cy];
	}
};

struct RMQ
{
	vector<pii> sta;
	void push(int i,int k)
	{
		while(sta.size() && sta.back().second >= k)
			sta.pop_back();
		sta.emplace_back(i,k);
	}
	pii query(int l) const
	{
		auto it = upper_bound(sta.begin(), sta.end(), pii{l, -inf});
		return it == sta.end()? pii{inf, inf}: *it;
	}
};

vector<int> get_cand(const Data &aa, const Data &bb)
{
	int d = aa.d;
	vector<int> type(d), need(d);
	for(int i=0; i<d; ++i)
	{
		type[i] = aa.cnt[i] <= bb.cnt[i]? 0: 1;
		need[i] = min(aa.cnt[i], bb.cnt[i]);
	}
	
	vector<pii> veca, vecb;
	for(int i=0; i<(int)aa.a.size(); ++i)
		if(type[aa.a[i]] == 0)
			veca.emplace_back(aa.a[i], aa.rnk[i]);
	for(int i=0; i<(int)bb.a.size(); ++i)
		if(type[bb.a[i]] == 1)
			vecb.emplace_back(bb.a[i], bb.rnk[i]);
	
	auto check = [&] (pii x,pii y)
	{
		return aa.check(x.first, x.second + 1, y.first, need[y.first] - y.second)
		    && bb.check(x.first, x.second + 1, y.first, need[y.first] - y.second);
	};
	
	vector<int> c;
	int i = 0, j = 0;
	while(i<(int)veca.size() && j<(int)vecb.size())
	{
		bool tx = check(veca[i], vecb[j]);
		bool ty = check(vecb[j], veca[i]);
		
		if(tx == ty) return {-1};
		
		if(tx) c.emplace_back(veca[i].first), ++i;
		else c.emplace_back(vecb[j].first), ++j;
	}
	
	while(i<(int)veca.size()) c.emplace_back(veca[i].first), ++i;
	while(j<(int)vecb.size()) c.emplace_back(vecb[j].first), ++j;
	
	return c;
}

vector<int> get_idx(vector<int> a, vector<int> b, int d)
{
	vector<int> pre(b.size()), bak(d, -1);
	for(int i=0; i<(int)b.size(); ++i)
	{
		pre[i] = bak[b[i]];
		bak[b[i]] = i;
	}
	
	vector< vector<int> > pos(d);
	for(int i=0; i<(int)a.size(); ++i)
		pos[a[i]].emplace_back(i);
	
	auto get_nxt = [&] (int i,int x)
	{
		auto it = lower_bound(pos[x].begin(), pos[x].end(), i);
		if(it == pos[x].end()) return (int)a.size();
		else return *it;
	};
	
	vector<int> res(b.size());
	RMQ rmq;
	
	for(int i=0; i<(int)b.size(); ++i)
	{
		int j = pre[i] < 0? -1: rmq.query(pre[i]).second;
		res[i] = get_nxt(j + 1, b[i]);
		rmq.push(i, res[i]);
	}
	
	return res;
}

vector<int> get_reversed(vector<int> a)
{
	reverse(a.begin(), a.end());
	return a;
}

bool is_invalid(const vector<int> &c)
{
	return c.size() == 1 && c[0] == -1;
}

bool is_subseq(const vector<int> &a, const vector<int> &c)
{
	int j = 0;
	for(int i=0; i<(int)a.size() && j<(int)c.size(); ++i)
		if(a[i] == c[j]) ++j;
	return j >= (int)c.size();
}

bool has_Abad(vector<int> a, vector<int> b, vector<int> c, int d, vector<int> type)
{
	vector<int> vb = get_idx(a, b, d);
	
//	printf("vb: ");
//	for(auto t: vb)
//		printf("%d ",t);
//	printf("\n");
	
	vector<int> pos_Aweak, pos_Bweak, vec_Aweak, id(c.size(), -1);
	for(int i=0; i<(int)c.size(); ++i)
	{
		if(type[c[i]] == 0)
		{
			id[i] = (int)pos_Aweak.size();
			pos_Aweak.emplace_back(i);
			vec_Aweak.emplace_back(c[i]);
		}
		else
		{
			pos_Bweak.emplace_back(i);
		}
	}
	
	vector<int> vc = get_idx(get_reversed(b), get_reversed(vec_Aweak), d);
	reverse(vc.begin(), vc.end());
	for(auto &t: vc)
		++t;
	
//	printf("vec_Aweak: ");
//	for(auto t: vec_Aweak)
//		printf("%d ",t);
//	printf("\n");
//	
//	printf("vc: ");
//	for(auto t: vc)
//		printf("%d ",t);
//	printf("\n");
	
	vector<int> cnt_Aweak(a.size()), cnt_Bweak(b.size());
	for(int i=0; i<(int)a.size(); ++i)
	{
		if(i) cnt_Aweak[i] = cnt_Aweak[i-1];
		if(type[a[i]] == 0) ++cnt_Aweak[i];
	}
	for(int i=0; i<(int)b.size(); ++i)
	{
		if(i) cnt_Bweak[i] = cnt_Bweak[i-1];
		if(type[b[i]] == 1) ++cnt_Bweak[i];
	}
	
	RMQ rmq;
	int cur = -1;
	for(int i=0; i<(int)b.size(); ++i) if(type[b[i]] == 1)
	{
		int j = vb[i], k = pos_Bweak[cnt_Bweak[i] - 1];
		if(j >= (int)a.size()) continue;
		
		while(cur < k)
		{
			++cur;
			if(id[cur] != -1)
				rmq.push(id[cur], vc[id[cur]]);
		}
		
		int mn = rmq.query(cnt_Aweak[j]).second;
		if(mn <= (int)b.size() - i)
			return 1;
	}
	return 0;
}

vector<int> ucs(vector<int> a, vector<int> b)
{
	vector<int> dsc;
	dsc.insert(dsc.end(), a.begin(), a.end());
	dsc.insert(dsc.end(), b.begin(), b.end());
	sort(dsc.begin(), dsc.end());
	dsc.erase(unique(dsc.begin(), dsc.end()), dsc.end());
	
	int d = (int)dsc.size();
	for(auto &t: a)
		t = lower_bound(dsc.begin(), dsc.end(), t) - dsc.begin();
	for(auto &t: b)
		t = lower_bound(dsc.begin(), dsc.end(), t) - dsc.begin();
	
//	printf("a: ");
//	for(auto t: a)
//		printf("%d ",t);
//	printf("\n");
//	
//	printf("b: ");
//	for(auto t: b)
//		printf("%d ",t);
//	printf("\n");
	
	Data aa(a, d), bb(b, d);
	
	auto c = get_cand(aa, bb);
	if(is_invalid(c)) return c;
	
	if(!is_subseq(a, c) || !is_subseq(b, c))
		return {-1};
	
	vector<int> type(d);
	for(int i=0; i<d; ++i)
		type[i] = aa.cnt[i] <= bb.cnt[i]? 0: 1;
	
//	printf("c: ");
//	for(auto t: c)
//		printf("%d ",dsc[t]);
//	printf("\n");
	
	if(has_Abad(a, b, c, d, type))
		return {-1};
	
	for(int i=0; i<d; ++i)
		type[i] ^= 1;
	if(has_Abad(b, a, c, d, type))
		return {-1};
	
	for(auto &t: c)
		t = dsc[t];
	
	return c;
}