#include <set>
#include <vector>
#include <cstdio>
#include <cassert>
using namespace std;
 
const int MAX = 200010;
 
int n;
pair<int,int> B[MAX];
bool C[MAX];
 
struct str_cmp {
    bool operator()(const int &a, const int &b) {
        if(B[a].first == B[b].first)
            return B[a].second < B[b].second;
        return B[a].first < B[b].first;
    }
};
 
set<int, str_cmp> S;
 
int dfs_loop(int v) {
    //printf("IN %d\n", v);
    if(B[v].second <= B[v].first) {
        C[v] = true;
        S.erase(S.find(v));
        //printf("OUT %d\n", v);
        return B[v].second;
    }
    while(true) {
        B[n + 3] = make_pair(B[v].second, -1);
        auto p = S.lower_bound(n + 3);
        if(p == S.end()) return 1 << 30;
        //printf("%d %d(%d, %d) %d(%d, %d) %d\n", v, *p, B[*p].first, B[*p].second, n + 3, B[n + 3].first, B[n + 3].second, B[v].second);
        //for(auto &x: S) printf("%d(%d, %d)\n", x, B[x].first, B[x].second);
        int g = dfs_loop(*p);
 
        if(g <= B[v].first) {
            C[v] = true;
            S.erase(S.find(v));
            //printf("OUT %d\n", v);
            return g;
        }
        else if(g == 1 << 30) {
            return g;
        }
    }
}
 
long long plan_roller_coaster(vector<int> s, vector<int> t) {
    n = (int) s.size();
 
    // CLEARING
    for(int i = 0; i < n; i++) C[i] = false;
    S.clear();
 
    int maxi = 0;
    for(int i = 0; i < n; i++) {
        B[i] = make_pair(s[i], t[i]);
        S.insert(i);
        maxi = max(maxi, t[i]);
        maxi = max(maxi, s[i]);
    }
    B[n] = make_pair(maxi + 1, 0);
    B[n + 1] = make_pair(-1, 1);
    S.insert(n);
    S.insert(n + 1);
    B[n + 2] = make_pair(maxi + 2, 0);
    auto p  = S.lower_bound(n + 2);
    assert(p == S.end());
    //for(auto &x: S) printf("%d(%d, %d)\n", x, B[x].first, B[x].second);
 
    dfs_loop(n + 1);
    int c = 0;
    for(int i = 0; i < n; i++) if(!C[i]) c++;
 
    return c;
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
