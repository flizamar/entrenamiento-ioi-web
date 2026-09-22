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
	
	Data aa(a, d), bb(b, d);
	
	auto c = get_cand(aa, bb);
	if(is_invalid(c)) return c;
	
	if(!is_subseq(a, c) || !is_subseq(b, c))
		return {-1};
	
	vector< vector<int> > nxt((int)c.size() + 1);
	nxt[(int)c.size()] = vector<int>(d, (int)c.size());
	for(int i=(int)c.size() - 1; i>=0; --i)
	{
		nxt[i] = nxt[i+1];
		nxt[i][c[i]] = i;
	}
	
	vector< vector<int> > dp(a.size() + 1, vector<int>(b.size() + 1, 0));
	for(int i=0; i<(int)a.size(); ++i)
		for(int j=0; j<(int)b.size(); ++j)
		{
			dp[i+1][j] = max(dp[i+1][j], dp[i][j]);
			dp[i][j+1] = max(dp[i][j+1], dp[i][j]);
			if(a[i] == b[j])
			{
				int k = dp[i][j];
				if(nxt[k][a[i]] >= (int)c.size())
					return {-1};
				dp[i+1][j+1] = max(dp[i+1][j+1], nxt[k][a[i]] + 1);
			}
		}
	
	for(auto &t: c)
		t = dsc[t];
	
	return c;
}