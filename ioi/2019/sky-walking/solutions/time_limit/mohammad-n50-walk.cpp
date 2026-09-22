#include <iostream>
#include <vector>
#include <memory.h>
#include <algorithm>
#include <set>
#include <map>

#define pb push_back
#define mp make_pair

using namespace std;

const int maxn = 1000000 + 10;
const long long inf = 1e18;
int n, m, X[maxn], Y[maxn], H[maxn], L[maxn], R[maxn], node;
long long dis[maxn];
vector<pair<int, long long>> adj[maxn];
map<pair<int, int>, int> nodes;
map<pair<int, int>, bool> mrk;

long long dijkstra(int source, int sink)
{
	set<pair<long long, int>> S;
	for (int i = 0; i <= node; i++)
		dis[i] = inf;
	dis[source] = 0;
	S.insert(mp(0, source));
	while (S.size())
	{
		set<pair<long long, int>>::iterator it = S.begin();
		long long dist = (*it).first;
		int v = (*it).second;
		S.erase(it);
		for (int i = 0; i < adj[v].size(); i++)
		{
			int u = adj[v][i].first;
			long long w = adj[v][i].second;
			if (dist + w < dis[u])
			{
				S.erase(mp(dis[u], u));
				dis[u] = dist + w;
				S.insert(mp(dis[u], u));
			}
		}
	}
	if (dis[sink] == inf)
		return -1;
	return dis[sink];
}

int add_map(int x, int y)
{
	if (!nodes[mp(x, y)])
		nodes[mp(x, y)] = ++node;
	return nodes[mp(x, y)];
}

void add_edge(int x1, int y1, int x2, int y2)
{
	int u = add_map(x1, y1);
	int v = add_map(x2, y2);
	if (x1 == x2)
	{
		adj[u].push_back(mp(v, abs(y2 - y1)));
		adj[v].push_back(mp(u, abs(y2 - y1)));
	}
	else
	{
		adj[u].push_back(mp(v, abs(x2 - x1)));
		adj[v].push_back(mp(u, abs(x2 - x1)));
	}
}

bool cmp(pair<int, int> aa, pair<int, int> bb){
	if(aa.second < bb.second) return true;
	if(aa.second > bb.second) return false;
	return aa.first < bb.first;
}

void build_graph()
{
	vector<pair<int, int> > pnt;
	for(int i = 0; i < n; i++){
		pnt.push_back(mp(X[i], 0));
		for(int j = 0; j < m; j++)
			if(i >= L[j] && i <= R[j] && H[i] >= Y[j]){
				pnt.push_back(mp(X[i], Y[j]));
				if(i == R[j]){
					mrk[mp(X[i], Y[j])] = true;
				}
			}
	}
	sort(pnt.begin(), pnt.end());
	for(int i = 1; i < pnt.size(); i++)
		if(pnt[i].first == pnt[i - 1].first)
			add_edge(pnt[i - 1].first, pnt[i - 1].second, pnt[i].first, pnt[i].second);
	sort(pnt.begin(), pnt.end(), cmp);
	for(int i = 1; i < pnt.size(); i++)
		if(pnt[i].second == pnt[i - 1].second && pnt[i].second != 0 && !mrk[pnt[i - 1]])
			add_edge(pnt[i - 1].first, pnt[i - 1].second, pnt[i].first, pnt[i].second);
}
bool cmp1(pair<int, pair<int, int> > aa, pair<int, pair<int, int> > bb){
	if(aa.first < bb.first)
		return true;
	if(aa.first > bb.first)
		return false;
	return aa.second.first < bb.second.first;

}



void init(){
	vector<pair<int, pair<int, int> > > tmp, nw;
	for(int i = 0; i < m; i++)
		tmp.push_back(mp(Y[i], mp(L[i], R[i])));
	sort(tmp.begin(), tmp.end(), cmp1);
	for(int i = 0; i < m; i++){
		if(tmp[i].second.first == tmp[i].second.second) continue;
		int j = i;
		int hei = tmp[i].first;
		int lo = tmp[i].second.first;
		int hi = tmp[i].second.second;
		while(tmp[j].first == hei && tmp[j].second.first <= hi && tmp[j].second.first >= lo){
			hi = max(hi, tmp[j].second.second);
			j++;
		}
		nw.push_back(mp(hei, mp(lo, hi)));
		i = j - 1;
	}
	m = nw.size();
	for (int i = 0; i < m; i++)
	{
		L[i] = nw[i].second.first;
		R[i] = nw[i].second.second;
		Y[i] = nw[i].first;
	}
}


long long min_distance(vector<int> XX, vector<int> HH, vector<int> LL, vector<int> RR, vector<int> YY, int S, int G)
{
	n = XX.size();
	m = LL.size();
	for (int i = 0; i < n; i++)
	{
		X[i] = XX[i];
		H[i] = HH[i];
	}
	for (int i = 0; i < m; i++)
	{
		L[i] = LL[i];
		R[i] = RR[i];
		Y[i] = YY[i];
	}
	init();
	build_graph();
	return dijkstra(add_map(X[S], 0), add_map(X[G], 0));
}
