//#include "grader.cpp"
#include "sphinx.h"
#include <bits/stdc++.h>

using namespace std;

const int MAX = 250 + 10;

int n, m;
vector< vector<int> >adj(MAX);

int vis[MAX];

void dfs(int node)
{
	vis[node] = 1;
	for(auto &child : adj[node])
	{
		if(vis[child])
			continue;
		dfs(child);
	}
}

int countComps(int node)
{
	vector<int>v;
	for(int i = 0; i <= node; ++i)
		v.push_back(-1);
	for(int i = node+1; i < n; ++i)
		v.push_back(n);
	int comps = perform_experiment(v);
	memset(vis, 0, sizeof(vis));
	for(int i = 0; i <= node; ++i)
		vis[i] = 1;
	int nComps = 0;
	for(int i = 0; i < n; ++i)
	{
		if(vis[i])
			continue;
		nComps++;
		dfs(i);
	}
	return (comps - nComps);
}

bool check(int node, int l, int r, vector< vector<int> >&CCs)
{
	vector<int>v;
	for(int i = 0; i < n; ++i)
		v.push_back(n);
	v[node] = -1;
	for(int i = l; i <= r; ++i)
	{
		for(auto &j : CCs[i])
			v[j] = -1;
	}
	int comps = perform_experiment(v);
	memset(vis, 0, sizeof(vis));
	vis[node] = 1;
	for(int i = l; i <= r; ++i)
	{
		for(auto &j : CCs[i])
			vis[j] = 1;
	}
	int nComps = r-l+1 + 1;
	for(int i = 0; i < n; ++i)
	{
		if(vis[i])
			continue;
		nComps++;
		dfs(i);
	}
	return (comps < nComps);
}

int mark[MAX], cmp[MAX];

vector< vector<int> >findCCs()
{
	vector< vector<int> >CCs, CCs2;
	vector<int>neigbours;
	CCs.push_back({0});
	int compCnt = 1;
	for(int i = 1; i < n; ++i)
	{
		neigbours.clear(), CCs2.clear();
		memset(mark, 0, sizeof(mark));
		int x = countComps(i);
		int neigbourCnt = compCnt+1 - x;
		int last = -1;
		for(int j = 0; j < neigbourCnt; ++j)
		{
			int l = last+1, r = CCs.size() - 1;
			int idx = r;
			while(l <= r)
			{
				int mid = (l + r) >> 1 ;
				if(check(i, l, mid, CCs))
					idx = mid, r = mid-1;
				else
					l = mid+1;
			}
			mark[idx] = 1;
			neigbours.push_back(idx);
			last = idx;
		}
		compCnt = x;
		for(int i = 0; i < CCs.size(); ++i)
		{
			if(mark[i])
				continue;
			CCs2.push_back(CCs[i]);
		}
		CCs2.push_back({i});
		for(auto &i : neigbours)
		{
			for(auto &node : CCs[i])
				CCs2.back().push_back(node);
		}
		CCs = CCs2;
		compCnt = CCs.size();
	}
	for(int i = 0; i < CCs.size(); ++i)
	{
		for(auto &node : CCs[i])
			cmp[node] = i;
	}
	return CCs;
}

vector< vector<int> >CCs;

vector< vector<int> >decomposeGraph()
{
	memset(mark, -1, sizeof(mark));
	int m = CCs.size();
	vector< vector<int> >decomposedAdj(m);
	for(int i = 0; i < m; ++i)
	{
		for(auto &node : CCs[i])
		{
			for(auto &child : adj[node])
			{
				int x = cmp[child];
				if(mark[x] == i)
					continue;
				decomposedAdj[i].push_back(x);
				mark[x] = i;
			}
		}
	} 
	return decomposedAdj;
}

vector< vector<int> >decomposedAdj, stAdj;

void dfs2(int node)
{
	vis[node] = 1;
	for(auto &child : decomposedAdj[node])
	{
		if(vis[child])
			continue;
		stAdj[node].push_back(child), stAdj[child].push_back(node);
		dfs2(child);
	}
}

void findST()
{
	memset(vis, 0, sizeof(vis));
	int m = CCs.size();
	stAdj = vector< vector<int> >(m+5);
	for(int i = 0; i < m; ++i)
	{
		if(vis[i])
			continue;
		dfs2(i);
	}
}

vector<int>s[2];

void findTwoSets(int node, int par, int col)
{
	s[col].push_back(node);
	for(auto &child : stAdj[node])
	{
		if(child == par)
			continue;
		findTwoSets(child, node, !col);
	}
}

void dfs3(int node, vector<int>&v)
{
	vis[node] = 1;
	for(auto &child : decomposedAdj[node])
	{
		if(vis[child] || v[child] != v[node])
			continue;
		dfs3(child, v);
	}
}

int countCompressedComps(int ty, int l, int r, int col, vector<int>&v)
{
	memset(vis, 0, sizeof(vis));
	for(int i = l; i <= r; ++i)
		vis[s[ty][i]] = 1;
	int cnt = 0;
	for(int i = 0; i < CCs.size(); ++i)
	{
		if(vis[i])
			continue;
		cnt++;
		dfs3(i, v);
	}
	return cnt;
}

bool check(int ty, int l, int r, int col)
{
	vector<int>v(n), v2(n);
	for(int i = 0; i < l; ++i)
	{
		v2[s[ty][i]] = n;
		for(auto &x : CCs[s[ty][i]])
			v[x] = n;
	}
	for(int i = l; i <= r; ++i)
	{
		v2[s[ty][i]] = -1;
		for(auto &x : CCs[s[ty][i]])
			v[x] = -1;
	}
	for(int i = r+1; i < s[ty].size(); ++i)
	{
		v2[s[ty][i]] = n;
		for(auto &x : CCs[s[ty][i]])
			v[x] = n;
	}
	for(auto &i : s[!ty])
	{
		v2[i] = col;
		for(auto &x : CCs[i])
			v[x] = col;
	}
	int comps = perform_experiment(v);
	return (comps < countCompressedComps(ty, l, r, col, v2) + r-l+1);
}

void solve(int col, int ty, int st, vector<int>&ans)
{
	if(st == s[ty].size())
		return;
	vector<int>v(n);
	for(int i = 0; i < st; ++i)
	{
		for(auto &node : CCs[s[ty][i]])
			v[node] = n;
	}
	for(int i = st; i < s[ty].size(); ++i)
	{
		for(auto &node : CCs[s[ty][i]])
			v[node] = -1;
	}
	for(auto &i : s[!ty])
	{
		for(auto &node : CCs[i])
			v[node] = col;
	}
	int comps = perform_experiment(v);
	if(comps == countCompressedComps(ty, st, s[ty].size()-1, col, v) + s[ty].size() - st)
		return;
	int l = st, r = s[ty].size() - 1;
	int idx = r;
	while(l <= r)
	{
		int mid = (l + r) >> 1;
		if(check(ty, st, mid, col))
			idx = mid, r = mid-1;
		else
			l = mid+1;
	}
	for(auto &node : CCs[s[ty][idx]])
		ans[node] = col;
	solve(col, ty, idx+1, ans);
}

vector<int>handleOneComp()
{
	vector<int>ans(n);
	for(int c = 0; c < n; ++c)
	{
		vector<int>v(n);
		v[0] = -1;
		for(int i = 1; i < n; ++i)
			v[i] = c;
		int compCnt = perform_experiment(v);
		if(compCnt == 1)
		{
			for(int i = 0; i < n; ++i)
				ans[i] = c;
			break;
		}
	}
	return ans;
}

vector<int>findColors()
{
	vector<int>ans(n, 0);
	for(int i = 0; i < n; ++i)
		solve(i, 0, 0, ans), solve(i, 1, 0, ans);
	return ans;
}

std::vector<int> find_colours(int N, std::vector<int> X, std::vector<int> Y) 
{
	n = N, m = X.size();
	for(int i = 0; i < m; ++i)
		adj[X[i]].push_back(Y[i]);
	CCs = findCCs();
	if(CCs.size() == 1)
		return handleOneComp();
	decomposedAdj = decomposeGraph();
	findST();
	findTwoSets(0, -1, 0);
	return findColors();
}
