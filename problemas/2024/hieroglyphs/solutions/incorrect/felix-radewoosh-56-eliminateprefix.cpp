//Mateusz Radecki, O(n+m)
#include "hieroglyphs.h"

#include <bits/stdc++.h>
using namespace std;

using ll=long long;
using pii=pair<int,int>;
using pll=pair<ll,ll>;
using vi=vector<int>;
using vll=vector<ll>;
const int nax=1007*1000;

int n, m;

int ilea[nax];
int ileb[nax];

int potrzebuje[nax];

vi pozy[nax];

int potrzebujewa[nax];
int potrzebujewb[nax];

vi licz_harde(vi a, vi b)
{
	n=a.size();
	m=b.size();
	for (int i=0; i<nax; i++)
		ilea[i]=ileb[i]=0;
	for (int i : a)
		ilea[i]++;
	for (int i : b)
		ileb[i]++;
	vector<pii> za, zb;
	for (int i=0; i<n; i++)
		if (ilea[a[i]]<=ileb[a[i]])
			za.push_back({a[i], i});
	for (int i=0; i<m; i++)
		if (ilea[b[i]]>ileb[b[i]])
			zb.push_back({b[i], i});
	
	{
		potrzebujewa[zb.size()]=n;
		int wsk=zb.size();
		for (int i=n-1; i>=0; i--)
		{
			if (wsk && zb[wsk-1].first==a[i])
			{
				wsk--;
				potrzebujewa[wsk]=i;
			}
		}
	}
	{
		potrzebujewb[za.size()]=m;
		int wsk=za.size();
		for (int i=m-1; i>=0; i--)
		{
			if (wsk && za[wsk-1].first==b[i])
			{
				wsk--;
				potrzebujewb[wsk]=i;
			}
		}
	}
	
	vi ret;
	int wska=0;
	int wskb=0;
	int gdz=0;
	for (int i : a)
	{
		if (wska<(int)za.size() && i==za[wska].first)
		{
			while(gdz<m && b[gdz]!=i)
				gdz++;
			if (gdz==m)
				return {-1};
			gdz++;
			ret.push_back(i);
			wska++;
		}
		if (wskb<(int)zb.size() && i==zb[wskb].first && gdz<=zb[wskb].second && potrzebujewb[wska]>zb[wskb].second)
		{
			ret.push_back(i);
			gdz=zb[wskb].second+1;
			wskb++;
		}
	}
	
	if ((int)ret.size()<(int)za.size()+(int)zb.size())
		return {-1};
	
	return ret;
}

vi licz_proste(vi a, vi b, int numwy)
{
	n=a.size();
	m=b.size();
	for (int i=0; i<nax; i++)
		ilea[i]=ileb[i]=0;
	for (int i : a)
		ilea[i]++;
	for (int i : b)
		ileb[i]++;
	vector<pii> za;
	for (int i=0; i<n; i++)
		if (ilea[a[i]]<=ileb[a[i]]-numwy)
			za.push_back({a[i], i});
	
	{
		potrzebuje[za.size()]=m;
		int wsk=za.size();
		for (int i=m-1; i>=0; i--)
		{
			if (wsk && za[wsk-1].first==b[i])
			{
				wsk--;
				potrzebuje[wsk]=i;
			}
		}
		if (wsk)
			return {-1};
	}
	
	for (int i=0; i<nax; i++)
		pozy[i].clear();
	for (int i=m-1; i>=0; i--)
		pozy[b[i]].push_back(i);
	
	vi ret;
	int wsk=0;
	int gdz=0;
	for (int i : a)
	{
		if (wsk<(int)za.size() && i==za[wsk].first)
		{
			while(gdz<m && b[gdz]!=i)
				gdz++;
			assert(gdz<m);
			gdz++;
			ret.push_back(i);
			wsk++;
		}
		else
		{
			while(!pozy[i].empty() && pozy[i].back()<gdz)
				pozy[i].pop_back();
			if (!pozy[i].empty() && potrzebuje[wsk]>pozy[i].back())
			{
				ret.push_back(i);
				gdz=pozy[i].back()+1;
			}
		}
	}
	
	return ret;
}

vi prosty(vi a, vi b)
{
	n=a.size();
	m=b.size();
	for (int i=0; i<nax; i++)
		pozy[i].clear();
	for (int i=m-1; i>=0; i--)
		pozy[b[i]].push_back(i);
	int gdz=0;
	vi ret;
	for (int i : a)
	{
		while(!pozy[i].empty() && pozy[i].back()<gdz)
			pozy[i].pop_back();
		if (!pozy[i].empty())
		{
			ret.push_back(i);
			gdz=pozy[i].back()+1;
		}
	}
	return ret;
}

int zawiera(vi kto, vi kogo)
{
	int wsk=0;
	for (int i : kto)
		if (wsk<(int)kogo.size() && i==kogo[wsk])
			wsk++;
	return wsk==(int)kogo.size();
}

vi _ucs(vi a, vi b)
{
	vi odp=licz_harde(a, b);
	vi raz=licz_proste(a, b, 0);
	vi dwa=licz_proste(b, a, 1);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	vi trz=licz_proste(a, b, 0);
	vi czt=licz_proste(b, a, 1);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	reverse(trz.begin(), trz.end());
	reverse(czt.begin(), czt.end());
	
	vi x=prosty(a, b);
	vi y=prosty(b, a);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	vi z=prosty(a, b);
	vi w=prosty(b, a);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	reverse(z.begin(), z.end());
	reverse(w.begin(), w.end());
	
	if (zawiera(odp, raz) && zawiera(odp, dwa) && zawiera(odp, trz) && zawiera(odp, czt) && zawiera(odp, x) && zawiera(odp, y) && zawiera(odp, z) && zawiera(odp, w))
		return odp;
	return {-1};
}

vi ucs(vi a, vi b) {
	//return _ucs(a, b);

    vi cpref;
    vi csuf;
    
    while (!a.empty() && !b.empty() && a.back() == b.back()) {
        csuf.push_back(a.back());
        a.pop_back();
        b.pop_back();
    }

    reverse(csuf.begin(), csuf.end());

    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());

    while (!a.empty() && !b.empty() && a.back() == b.back()) {
        cpref.push_back(a.back());
        a.pop_back();
        b.pop_back();
    }


    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());

    vi rc = _ucs(a, b);
    if (rc == vi({-1})) return rc;

    vi c;
    for (int x : cpref) c.push_back(x);
    for (int x : rc) c.push_back(x);
    for (int x : csuf) c.push_back(x);
    return c;
}
