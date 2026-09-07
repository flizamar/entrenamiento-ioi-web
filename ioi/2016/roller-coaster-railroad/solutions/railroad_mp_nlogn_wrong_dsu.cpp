#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
#define mp make_pair
#define pb push_back
#define fs first
#define sc second
typedef long long int64;
const int INF = (int) 1e9;

int dsu_get(vector<int>& p, int u) {
	return (u == p[u]) ? u : (u = dsu_get(p, p[u]));
}

bool dsu_union(vector<int>& p, int u, int v) {
	u = dsu_get(p, u), v = dsu_get(p, v);
	p[u] = v;
	return (u != v);
}

int64 plan_roller_coaster(vector<int> s, vector<int> t) {
    int n = (int) s.size();
    vector< pair< int, pair< int, int > > > e, edges;
    for (int i = 0; i < n; ++i) {
        e.pb(mp(s[i], mp(1, i)));
        e.pb(mp(t[i], mp(-1, i)));
    }
    e.pb(mp(INF, mp(1, n)));
    e.pb(mp(1, mp(-1, n)));
    n++;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = i;
    }
    sort(e.begin(), e.end());
    int64 res = 0;
	for (int i = 0, delta = 0; i + 1 < (int) e.size(); ++i) {
		delta += e[i].sc.fs;
		res += max(0, delta) * (int64) (e[i + 1].fs - e[i].fs);
		edges.pb(mp(e[i + 1].fs - e[i].fs, mp(e[i].sc.sc, e[i + 1].sc.sc)));
		if ((e[i + 1].fs == e[i].fs) || (delta != 0))
			dsu_union(p, e[i].sc.sc, e[i + 1].sc.sc);
	}
	sort(edges.begin(), edges.end());
	for (int i = 0; i < (int) edges.size(); ++i)
		if (dsu_union(p, edges[i].sc.fs, edges[i].sc.sc))
			res += edges[i].fs;
	return res;
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
