#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

using namespace std;

#define pb push_back
#define mp make_pair
#define fs first
#define sc second

const int maxN = 1000 * 1000;
const long long inf = (long long) 1e18;


long long dcrd[maxN];
long long depth[maxN];
int from[maxN];
int used_cnt = 0;
vector <int> important;

int m, c;

bool check(long long mid, int& a, int& b) {
	long long suml = -inf, sumr = inf;
	long long difl = -inf, difr = inf;

	// cross areas
	int p = 0;
	for (int i = 0; i < (int) important.size(); i++) {
		while (p < i && depth[important[i]] + depth[important[p]] + dcrd[important[i]] - dcrd[important[p]] > mid)
			p++;
		if (p > 0) {
			// First not satisfied by distance mid
			long long rad = mid - (depth[important[i]] + depth[important[p - 1]] + c);
			suml = max(suml, dcrd[important[i]] + dcrd[important[p - 1]] - rad);
			sumr = min(sumr, dcrd[important[i]] + dcrd[important[p - 1]] + rad);
			difl = max(difl, dcrd[important[i]] - dcrd[important[p - 1]] - rad);
			difr = min(difr, dcrd[important[i]] - dcrd[important[p - 1]] + rad);

			// First vertex of the diameter
			rad = mid - (depth[important[i]] + depth[important[0]] + c);
			suml = max(suml, dcrd[important[i]] + dcrd[important[0]] - rad);
			sumr = min(sumr, dcrd[important[i]] + dcrd[important[0]] + rad);
			difl = max(difl, dcrd[important[i]] - dcrd[important[0]] - rad);
			difr = min(difr, dcrd[important[i]] - dcrd[important[0]] + rad);
		}
	}

	if (suml > sumr || difl > difr)
		return false;
	// find any valid point inside the area
	p = 0;
	for (int i = 0; i < m - 1; i++) {
		while (p <= i || (p < m - 1 && dcrd[p] + dcrd[i] <= sumr && dcrd[p] - dcrd[i] <= difr))
			p++;
		while (p > i + 1 && (dcrd[p] + dcrd[i] > sumr || dcrd[p] - dcrd[i] > difr))
			p--;
		if (dcrd[i] + dcrd[p] >= suml && dcrd[i] + dcrd[p] <= sumr && dcrd[p] - dcrd[i] >= difl && dcrd[p] - dcrd[i] <= difr) {
			a = i;
			b = p;

			return true;
		}
	}

	return false;
}

int main() {
//    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);

    // read input data
    scanf("%d%d", &m, &c);
	dcrd[0] = 0;
    for (int i = 0; i < m - 1; i++) {
    	int w;
    	scanf("%d", &w);
		dcrd[i + 1] = dcrd[i] + w;
    }
    for (int i = 0; i < m; i++) scanf("%lld", &depth[i]);

    // this is needed to reduce complexity from n log n log d to n log d
	for (int i = 0; i < m; i++) {
		while (!important.empty() && depth[important.back()] + dcrd[i] - dcrd[important.back()] < depth[i])
			important.pop_back();
		if (important.empty() || depth[important.back()] <= depth[i] + dcrd[i] - dcrd[important.back()])
			important.pb(i);
	}

	long long lb = 1;
	long long rb = inf;
    int ans_a = 1, ans_b = 2;
    while (lb < rb) {
    	long long mid = (lb + rb) / 2;
    	if (check(mid, ans_a, ans_b))
    		rb = mid;
    	else
    		lb = mid + 1;
    }

    check(lb, ans_a, ans_b);
    
    // BEGIN SECRET
    puts("14e047d7a2907b9034950b074822b302");
    // END SECRET
    
    
    printf("%lld\n", lb);
//     printf("%d %d\n", ans_a + 1, ans_b + 1);
    
    return 0;
}
