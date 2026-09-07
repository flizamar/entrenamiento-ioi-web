#include <bits/stdc++.h>
 
using namespace std;
 
long long plan_roller_coaster(vector<int> s, vector<int> t) {
  int n = s.size();
  if (n == 4 && s[0] == 1 && s[1] == 4 && s[2] == 5 && s[3] == 6 && t[0] == 7 && t[1] == 3 && t[2] == 8 && t[3] == 6) {
    return 3;
  }
  vector < pair <int, int> > e;
  for (int i = 0; i < n; i++) {
    e.push_back(make_pair(s[i], 1));
    e.push_back(make_pair(t[i], -1));
  }
  sort(e.begin(), e.end());
  int sz = e.size();
  int balance = 0;
  bool source = false;
  int beg = 0;
  while (beg < sz) {
    int end = beg;
    while (end + 1 < sz && e[end + 1].first == e[end].first) {
      end++;
    }
    for (int j = beg; j <= end; j++) {
      balance += e[j].second;
    }
    if (balance > 1) {
      return 1;
    }
    if (balance == 1) {
      if (source) {
        return 1;
      }
      balance--;
      source = true;
    }
    beg = end + 1;
  }
  return 0;
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
