//In the Name of God
//Ya Ali

#include <bits/stdc++.h>

#define lng int64_t
#define pll pair<lng, lng>

#define err(A) cout << #A << " = " << (A) << endl

using namespace std;

const lng inf = 1e18;
const int maxn = 100100;

int tof;

vector<int> x, h, l, r, y;
int s, g;
int n, m;
vector<int> ll[maxn], rr[maxn];

lng mn[maxn];
bool mw[maxn], mx[maxn];

void set_tof()
{
  tof = 20;
}


lng solve()
{
  for (int i = 0; i < m; i ++)
    {
      ll[l[i]].push_back(i);
      rr[r[i]].push_back(i);
    }
  set<pll> sm, sd;

  if (ll[0].size() == 0)
    return -1;
  
  sm.insert({0, 0}); // <min_dis, y>
  sd.insert({0, 0}); // <y, min_dis>

  for (int i = 0; i < n; i ++)
    {
      set<pll> tm, td;

      for (int j: ll[i])
	{
	  mn[j] = inf;

	  {// down
	    int ct = tof;
	    for (auto it = sd.begin(); it != sd.end() and ct; it ++, ct --)
	      mn[j] = min(mn[j], abs(it -> first - y[j]) + it -> second);
	  }
	  {// up
	    int ct = tof;
	    for (auto it = sd.rbegin(); it != sd.rend() and ct; it ++, ct --)
	      mn[j] = min(mn[j], abs(it -> first - y[j]) + it -> second);
	  }
	  {// goods
	    int ct = tof;
	    for (auto it = sm.begin(); it != sm.end() and ct; it ++, ct --)
	      mn[j] = min(mn[j], abs(it -> second - y[j]) + it -> first);
	  }
	  {// nearby
	    auto it = sd.lower_bound({y[j], 0});
	    int ct = tof / 2;
	    while (it != sd.begin() and ct --)
	      it --;
	    ct = tof;
	    for (; it != sd.end() and ct; it ++, ct --)
	      mn[j] = min(mn[j], abs(it -> first - y[j]) + it -> second);
	  }
	  
	  tm.insert({mn[j], y[j]});
	  td.insert({y[j], mn[j]});
	}

      if (i != n - 1)
	for (int j: rr[i])
	  {
	    sm.erase({mn[j], y[j]});
	    sd.erase({y[j], mn[j]});
	  }
      if (i == 0)
	{
	  sd.erase({0, 0});
	  sm.erase({0, 0});
	}
      
      for (pll p: tm)
	sm.insert({p.first, p.second});
      for (pll p: td)
	sd.insert({p.first, p.second});
    }
  
  lng res = inf;
  for (pll p: sd)
    res = min(res, lng(p.second) + p.first);
  for (pll p: sm)
    res = min(res, lng(p.first) + p.second);
  
  return res + x[n - 1] - x[0];
}

void remove_trash()
{
  for (int i = 0; i < m; i ++)
    {
      rr[r[i]].push_back(i);
      ll[l[i]].push_back(i);
    }
  
  mx[n - 1] = true;
  set<pll> sw;
  set<pll> ww;
  
  for (int i = n; i --; )
    {
      if (sw.size() and sw.begin() -> first <= h[i])
	mx[i] = true;

      for (int j: rr[i])
	ww.insert({y[j], j});
      
      if (mx[i])
	while (ww.size() and ww.begin() -> first <= h[i])
	  {
	    auto it = ww.begin();
	    mw[it -> second] = true;
	    
	    if (l[it -> second] != i)
	      r[it -> second] = i;
	    
	    assert(l[it -> second] <= r[it -> second]);
	    assert(h[r[it -> second]] >= y[it -> second]);
	    sw.insert(*it);
	    ww.erase(it);
	  }
      for (int j: ll[i])
	if (mw[j])
	  {
	    assert(sw.count({y[j], j}));
	    sw.erase({y[j], j});
	  }
	else
	  {
	    assert(ww.count({y[j], j}));
	    ww.erase({y[j], j});
	  }
    }
  memset(mx, false, sizeof mx);
  for (int i = 0; i < n; i ++)
    {
      ll[i].resize(0);
      rr[i].resize(0);
    }
  vector<int> L, R, Y;
  for (int i = 0; i < m; i ++)
    if (mw[i])
      {
	L.push_back(l[i]);
	R.push_back(r[i]);
	Y.push_back(y[i]);
      }
  l.swap(L);
  r.swap(R);
  y.swap(Y);
  m = l.size();
}

void init(vector<int> x, vector<int> h, vector<int> l, vector<int> r, vector<int> y, int s, int g)
{
  ::x = x, ::h = h, ::l = l, ::r = r, ::y = y, ::s = s, ::g = g; n = x.size(), m = l.size();
}

lng min_distance(vector<int> x, vector<int> h, vector<int> l, vector<int> r, vector<int> y, int s, int g)
{
  init(x, h, l, r, y, s, g);
  remove_trash();
  set_tof();
  return solve();
}

