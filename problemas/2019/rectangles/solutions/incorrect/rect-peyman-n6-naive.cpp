#include "rect.h"
#include <bits/stdc++.h>
#define X first
#define Y second
#define MP make_pair
using namespace std;
typedef pair<int,int> pii;

const int maxn = 3000;
int n,m;
long long ans;

long long count_rectangles(vector<vector<int> > a)
{
	n = a.size();
	m = a[0].size();
	for(int i1=1;i1<n-1;i1++)
		for(int i2=i1;i2<n-1;i2++)
			for(int j1=1;j1<m-1;j1++)
				for(int j2=j1;j2<m-1;j2++)
				{
					bool ok=true;
					for(int x=i1;x<=i2;x++)
						for(int y=j1;y<=j2;y++)
							if(a[x][y]>=a[x][j1-1] || a[x][y]>=a[x][j2+1] || a[x][y]>=a[i1-1][y] || a[x][y]>=a[i2+1][y])
								ok=false;
					ans += ok;
				}
	return ans;
}
