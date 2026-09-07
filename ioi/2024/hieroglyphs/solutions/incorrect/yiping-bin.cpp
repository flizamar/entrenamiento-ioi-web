#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

bool is_subseq(const vector<int> &a, const vector<int> &c)
{
	int j = 0;
	for(int i=0; i<(int)a.size() && j<(int)c.size(); ++i)
		if(a[i] == c[j]) ++j;
	return j >= (int)c.size();
}

bool is_single(const vector<int> &a,int l,int r)
{
	for(int i=l; i<(int)a.size()-r; ++i)
		if(a[i] != a[l]) return 0;
	return 1;
}

vector<int> ucs(vector<int> a, vector<int> b)
{
	if(is_subseq(a,b)) return b;
	if(is_subseq(b,a)) return a;
	
	int l = 0, r = 0;
	vector<int> pre, suf;
	while(a[l] == b[l]) ++l;
	while(a[(int)a.size()-r-1] == b[(int)b.size()-r-1]) ++r;
	
	if(is_single(b,l,r)) swap(a,b);
	if(!is_single(a,l,r)) return {-1};
	
	vector<int> c(a.begin(), a.begin() + l);
	c.insert(c.end(), count(b.begin() + l, b.end() - r, a[l]), a[l]);
	c.insert(c.end(), a.end() - r, a.end());
	
	return c;
}