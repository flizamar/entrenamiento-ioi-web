#include<bits/stdc++.h>
#include"hieroglyphs.h"
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

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
	int get_next(int i,int x) const
	{
		auto it = lower_bound(pos[x].begin(), pos[x].end(), i);
		return it == pos[x].end()? (int)a.size(): *it;
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

vector<int> ucs(vector<int> a, vector<int> b)
{
	int d = 0;
	for(auto t: a)
		d = max(d, t + 1);
	for(auto t: b)
		d = max(d, t + 1);
	
	Data aa(a, d), bb(b, d);
	
	auto c = get_cand(aa, bb);
	if(is_invalid(c)) return c;
	
	if(!is_subseq(a, c)) return {-1};
	if(!is_subseq(b, c)) return {-1};
	
	Data cc(c, d);
	
	const int LIM = 100;
	
	vector< vector<int> > dp(LIM+1, vector<int>(LIM+1));
	
	int lst = 0;
	
	for(int al=0,ar, bl=0,br; al<(int)a.size() && bl<(int)b.size(); al=ar, bl=br)
	{
		ar = min((int)a.size(), al + LIM);
		br = min((int)b.size(), bl + LIM);
		
		for(int i=0; i<LIM; ++i)
			fill(dp[i].begin(), dp[i].end(), lst);
		
		for(int i=0; i<=ar-al; ++i)
			for(int j=0; j<=br-bl; ++j)
			{
				if(i) dp[i][j] = max(dp[i][j], dp[i-1][j]);
				if(j) dp[i][j] = max(dp[i][j], dp[i][j-1]);
				if(i && j && a[al+i-1] == b[bl+j-1])
				{
					dp[i][j] = max(dp[i][j], cc.get_next(dp[i-1][j-1], a[al+i-1]) + 1);
					if(dp[i][j] > (int)c.size())
						return {-1};
				}
			}
		
		lst = dp[ar-al][br-bl];
	}
	
	return c;
}