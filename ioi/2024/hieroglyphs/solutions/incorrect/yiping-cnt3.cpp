#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

struct BIT
{
	int n;
	vector<int> tree;
	#define lowbit(x) ((x)&-(x))
	BIT(int _n)
	{
		n = _n;
		tree = vector<int>(n+1);
	}
	void update(int x,int k)
	{
		++x;
		while(x<=n)
			tree[x] += k, x += lowbit(x);
	}
	int query(int x)
	{
		++x;
		int res = 0;
		while(x)
			res += tree[x], x ^= lowbit(x);
		return res;
	}
	int query(int l,int r)
	{
		return query(r) - query(l-1);
	}
};

vector<int> remove_useless(vector<int> a,vector<int> b)
{
	vector<int> res;
	set<int> bak(b.begin(), b.end());
	for(auto t: a) if(bak.count(t))
		res.emplace_back(t);
	return res;
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
	a = remove_useless(a, b);
	b = remove_useless(b, a);
	
	vector<int> dsc = a;
	sort(dsc.begin(), dsc.end());
	dsc.erase(unique(dsc.begin(), dsc.end()), dsc.end());
	
	int d = (int)dsc.size();
	auto getdsc = [&] (int x)
	{
		return lower_bound(dsc.begin(), dsc.end(), x) - dsc.begin();
	};
	
	for(auto &t: a)
		t = getdsc(t);
	for(auto &t: b)
		t = getdsc(t);
	
	vector< vector<int> > posA(d), posB(d);
	for(int i=0; i<(int)a.size(); ++i)
		posA[a[i]].emplace_back(i);
	for(int i=0; i<(int)b.size(); ++i)
		posB[b[i]].emplace_back(i);
	
	auto cmp = [&] (int x,int y) -> int
	{
		bool usex = posA[x][0] < posA[y].back() && posB[x][0] < posB[y].back();
		bool usey = posA[y][0] < posA[x].back() && posB[y][0] < posB[x].back();
		if(usex == usey) return -1;
		return usex;
	};
	
	auto getc = [&] (void) -> vector<int>
	{
		vector<int> vecA, vecB;
		for(auto t: a) if(posA[t].size() <= posB[t].size())
			vecA.emplace_back(t);
		for(auto t: b) if(posA[t].size() > posB[t].size())
			vecB.emplace_back(t);
		
		int i = 0, j = 0;
		vector<int> c;
		while(i<(int)vecA.size() && j<(int)vecB.size())
		{
			int t = cmp(vecA[i], vecB[j]);
			if(t == -1) return {-1};
			if(t) c.emplace_back(vecA[i++]);
			else c.emplace_back(vecB[j++]);
		}
		
		while(i<(int)vecA.size()) c.emplace_back(vecA[i++]);
		while(j<(int)vecB.size()) c.emplace_back(vecB[j++]);
		
		return c;
	};
	
	auto c = getc();
	if(c.size() && c[0] == -1) return {-1};
	
	if(!is_subseq(a, c)) return {-1};
	if(!is_subseq(b, c)) return {-1};
	
	// Only the following case is invalid:
	// A: 121
	// B: 212
	
	BIT tree(b.size());
	
	for(int i=0; i<(int)a.size(); ++i)
	{
		int x = a[i];
		if(posA[x].size() == 2)
		{
			if(i == posA[x][0])
				tree.update(posB[x][0], 1);
			else
				tree.update(posB[x][0], -1);
		}
		if(posB[x].size() == 2)
		{
			if(tree.query(posB[x][0], posB[x][1]))
				return {-1};
		}
	}
	
	for(auto &t: c)
		t = dsc[t];
	
	return c;
}