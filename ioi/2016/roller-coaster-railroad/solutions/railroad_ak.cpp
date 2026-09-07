#include <bits/stdc++.h>
 
#define SZ(AA) ((int)((AA).size()))
#define ALL(AA) (AA).begin(), (AA).end()
 
using namespace std;
 
typedef vector<int> VI;
typedef pair<int, int> PII;
typedef long long LL;
typedef long double LD;
 
LL plan_roller_coaster(vector<int> s, vector<int> t) {
  int n = SZ(s);
  vector<PII> a, b;
  a.push_back({1, -1});
  b.push_back({max(*max_element(ALL(s)), *max_element(ALL(t))), n});
  for (int i = 0; i < n; ++i) {
    a.push_back({t[i], i});
    b.push_back({s[i], i});
  }
  sort(ALL(a));
  sort(ALL(b));
  for (int i = 0; i < SZ(a); ++i) {
    if (a[i].first > b[i].first) {
      return 1;
    }
  }
  VI ra(n);
  for (int i = 0; i < SZ(a); ++i) {
    if (a[i].second >= 0 && a[i].second < n) {
      ra[a[i].second] = i;
    }
  }
  vector<bool> vis(SZ(a));
  VI Q;
  vis[0] = true; Q.push_back(0);
  for (int i = 0; i < SZ(Q); ++i) {
    int v = Q[i];
    VI can;
    if (b[v].second != n) {
      can.push_back(ra[b[v].second]);
    }
    if (v > 0 && b[v - 1].first >= a[v].first) {
      can.push_back(v - 1);
    }
    if (v < SZ(a) - 1 && b[v].first >= a[v + 1].first) {
      can.push_back(v + 1);
    }
    for (int w : can) {
      if (!vis[w]) {
        vis[w] = true;
        Q.push_back(w);
      }
    }
  }
  return SZ(Q) == SZ(a) ? 0 : 1;
}

//BEGIN CUT

int main() {
	int n, need_answer;
	scanf("%d%d", &n, &need_answer);
	std::vector<int> s(n), t(n);
	for (int i = 0; i < n; ++i)
		scanf("%d%d", &s[i], &t[i]);
	long long ans = plan_roller_coaster(s, t);
	
	// BEGIN SECRET
	puts("14e047d7a2907b9034950b074822b302");
	// END SECRET
	
	printf("%lld\n", ans);
	
	return 0;
}

//END CUT
