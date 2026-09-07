#include <bits/stdc++.h>
#define ll long long
using namespace std;

const ll N = 1e5 + 10, inf = 2e9;
int n, m;

struct SkyWalk {
	int l, r, h;
}s[N];

bool cmp(SkyWalk s1, SkyWalk s2)
{
	return s1.l < s2.l;
}

ll tof_value(SkyWalk s1, SkyWalk s2)
{
	if(s1.h > s2.h) return s1.h - s2.h + inf;
	return s1.h - s2.h;
}

ll solve1()
{
	int id = 0;
	ll ans = s[0].h;
	while(s[id].r < n - 1)
	{
		int mn_id = id;
		ll mn = inf * 100;
		for(int j = id + 1; j < m && s[j].l <= s[id].r; j++)
		{
			if(s[j].r > s[id].r && tof_value(s[j], s[id]) < mn)
			{
				mn = tof_value(s[j], s[id]);
				mn_id = j;
			}
		}
		if(mn_id == id) return -1;
		if(s[id].h < s[mn_id].h) ans += s[mn_id].h - s[id].h;
		id = mn_id;
	}
	return 2 * ans;
}

long long min_distance(vector<int> x, vector<int> h, vector<int> l, vector<int> r, vector<int> y, int st, int en)
{
	n = x.size();
	m = l.size();
	for(int i = 0; i < n; i++) assert(h[i] == h[0]);
	assert(st == 0);
	assert(en == n - 1);
	for(int i = 0; i < m; i++) s[i] = {l[i], r[i], y[i]};
	sort(s, s + m, cmp);
	ll ans = solve1();
	if(ans == -1) return -1;
	return x[n - 1] - x[0] + ans;
}

