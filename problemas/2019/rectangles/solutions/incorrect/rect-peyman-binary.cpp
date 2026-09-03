#include "rect.h"
#include <bits/stdc++.h>
#define X first
#define Y second
#define MP make_pair
using namespace std;
typedef pair<int,int> pii;

const int maxn = 3000 + 10;
const int inf = 3000 + 10;
int n, m, t, MINX, MINY, MAXX, MAXY;
long long ans;
bool mark[maxn * maxn];
vector<int>G[maxn * maxn];

int f(int x,int y)
{
  return x*m+y;
}

pii g(int x)
{
  return pii(x/m, x%m);
}

void add_edge(int v, int u)
{
  G[v].push_back(u);
  G[u].push_back(v);
}

void dfs(int v)
{
  mark[v] = true;
  t++;
  int x=g(v).X, y=g(v).Y;
  MINX=min(MINX,x);
  MINY=min(MINY,y);
  MAXX=max(MAXX,x);
  MAXY=max(MAXY,y);
  for(int i=0;i<G[v].size();i++)
    {
      int u=G[v][i];
      if(!mark[u])
	dfs(u);
    }
}

long long count_rectangles(vector<vector<int> > a)
{
  n = a.size();
  m = a[0].size();
  for(int i=0;i<n;i++)
    for(int j=0;j<m;j++)
      {
	if(a[i][j])
	  mark[f(i,j)] = true;
	if(i<n)
	  add_edge(f(i,j), f(i+1,j));
	if(j<m)
	  add_edge(f(i,j), f(i,j+1));
      }
  for(int i=0;i<n*m;i++)
    if(!mark[i])
      {
	t=0;
	MINX=MINY=inf;
	MAXX=MAXY=0;
	dfs(i);
	if(MINX>0 && MAXX<n-1 && MINY>0 && MAXY<m-1 && t==(MAXX-MINX+1)*(MAXY-MINY+1))
	  ans ++;
      }
  return ans;
}
