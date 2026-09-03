#include "rect.h"
#include <bits/stdc++.h>
#define X first
#define Y second
#define MP make_pair
using namespace std;
typedef pair<int,int> pii;

const int maxn = 300;
int n,m, row[maxn][maxn][maxn], col[maxn][maxn][maxn];
long long ans;

long long count_rectangles(vector<vector<int> > a)
{
  n = a.size();
  m = a[0].size();
  for(int i=0;i<n;i++)
    for(int j1=1;j1<m-1;j1++)
      for(int j2=j1;j2<m-1;j2++)
	{
	  row[i][j1][j2] = true;
	  for(int y=j1;y<=j2;y++)
	    if(a[i][y]>=a[i][j1-1] || a[i][y]>=a[i][j2+1])
	      row[i][j1][j2] = false;
	}

  for(int j=0;j<m;j++)
    for(int i1=1;i1<n-1;i1++)
      for(int i2=1;i2<n-1;i2++)
	{
	  col[j][i1][i2] = true;
	  for(int x=i1;x<=i2;x++)
	    if(a[x][j]>=a[i1-1][j] || a[x][j]>=a[i2+1][j])
	      col[j][i1][i2] = false;
	}
  
  for(int i1=1;i1<n-1;i1++)
    for(int i2=i1;i2<n-1;i2++)
      for(int j1=1;j1<m-1;j1++)
	for(int j2=j1;j2<m-1;j2++)
	  {
	    bool ok=true;
	    for(int x=i1;x<=i2;x++)
	      ok &= row[x][j1][j2];
	    for(int y=j1;y<=j2;y++)
	      ok &= col[y][i1][i2];
	    ans += ok;
	  }
  return ans;
}
