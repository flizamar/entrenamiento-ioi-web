//In the name of God
//Peyman Jabbarzade Ganje
//Boarder shouldn't be equal with inside
#include<bits/stdc++.h>
#define X first
#define Y second
using namespace std;
typedef pair<int, int> pii;

const int maxn = 3000 + 10, maxx=1e7;
int n,m;
long long ans;
vector<int>vec, nozoli, will_remove;
map<pii, int> fp[maxn];
vector<pii> sp[2][maxn][maxn];

void add_pair(int type, int lvl, int l, int r)
{
	int tmp = 1;
	if(fp[lvl].find(pii(l,r)) != fp[lvl].end())
		tmp += fp[lvl][pii(l,r)];

	if(type == 0) {
		if(r-l-1>0)
			sp[type][lvl+1][r].push_back(pii(r-l-1, tmp));
	}
	else {
		if(r-l-1>0)
			sp[type][r][lvl+1].push_back(pii(tmp, r-l-1));
	}

	fp[lvl+1][pii(l,r)] = tmp;
}

void extract_pairs(int type, int lvl)
{
	nozoli.clear();
	for(int i=0;i<vec.size();i++)
	{
		int last=-1;
		while(nozoli.size() && vec[nozoli.back()] < vec[i])
		{
			if(vec[nozoli.back()] > last)
				add_pair(type, lvl, nozoli.back(), i);
			last = vec[nozoli.back()];
			nozoli.pop_back();
		}
		if(nozoli.size()) add_pair(type, lvl, nozoli.back(), i);
		nozoli.push_back(i);
	}
}

long long count_rectangles(vector<vector<int> >a)
{
	n = a.size();
	m = a[0].size();

	for(int i=0;i<n;i++)
	{
		vec.clear();
		for(int j=0;j<m;j++)
			vec.push_back(a[i][j]);
		extract_pairs(0,i);
	}
	for(int i=0;i<=n;i++)
		fp[i].clear();

	for(int j=0;j<m;j++)
	{
		vec.clear();
		for(int i=0;i<n;i++)
			vec.push_back(a[i][j]);
		extract_pairs(1,j);
	}

//	int tt = 0;

	for(int i=0;i<=max(n,m);i++) {
		for(int j=0;j<=max(n,m);j++) {
			if(sp[0][i][j].size() && sp[1][i][j].size())
			{
				// sp[0].X fix sp[1].X moteghaier
				for(int x=0;x<sp[0][i][j].size();x++)
					for(int y=0;y<sp[1][i][j].size();y++) {
//						tt++;
						if(sp[0][i][j][x].X <= sp[1][i][j][y].X && sp[0][i][j][x].Y >= sp[1][i][j][y].Y)
							ans ++;
					}
			}
		}
	}
//	cerr << " $$ " << tt << endl;
	return ans;
}
