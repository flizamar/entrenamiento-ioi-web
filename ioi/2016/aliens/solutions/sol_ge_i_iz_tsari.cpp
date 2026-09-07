#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

#define pb push_back
#define mp make_pair
#define fs first
#define sc second

const double pi = acos(-1.0);
const int generation = 30000;
const int size = 4050;
const int iters = 1000 * 1000 * 300;
const long long inf = (long long) 1e18;

vector <pair <int, int> > segs;

random_device rd;
mt19937 rgenerator(rd());
    
int genRandInt(int lb, int rb) {
    return lb + rgenerator() % (rb - lb + 1);
}

long long sqr(long long x) {
    return x * x;
}

long long get_value(vector <int>& ans) {
    long long res = 0;
    int lst = 0;
    int k = (int) ans.size();
    for (int i = 1; i < k; i++) {
        res += sqr(segs[ans[i] - 1].sc - segs[lst].fs + 1);
        res -= sqr(max(0, segs[ans[i] - 1].sc - segs[ans[i]].fs + 1));
//        cerr << res << endl;
        lst = ans[i];        
    }    

    res += sqr(segs.back().sc - segs[lst].fs + 1);

    return res;
}    

long long ans[size][size];
long long cost[size][size];

long long take_photos(int n, int m, int k, vector <int> r, vector <int> c) {
    vector <pair <int, int> > pts;
    for (int i = 0; i < n; i++)
        pts.pb(mp(min(r[i], c[i]), -max(r[i], c[i])));
    sort(pts.begin(), pts.end());

    int cmx = -1;
    for (int i = 0; i < n; i++) {
        if (-pts[i].sc > cmx) {
            segs.pb(pts[i]);
            segs.back().sc *= -1;
            cmx = -pts[i].sc;
        }
    }

    n = (int) segs.size();
    k = min(n, k);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            cost[j][i] = sqr(segs[j - 1].sc - segs[i].fs + 1) - sqr(max(0, segs[j - 1].sc - segs[j].fs + 1));
        }
        cost[n][i] = sqr(segs.back().sc - segs[i].fs + 1);
    }

    for (int i = 0; i <= k; i++)
        for (int j = 0; j <= n; j++)
            ans[i][j] = inf;
    ans[0][0] = 0;
    
//    for (int i = 1; i <= k; i++) {
//        for (int j = 0; j < n; j++)
//            for (int q = j + 1; q <= n; q++)
//                ans[i][q] = min(ans[i][q], ans[i - 1][j] + cost[j][q]);
//    }
    int magic = iters / (n * k);
//    int magic = 1;

    for (int i = 1; i <= k; i++) {
        int p = 0;
        for (int j = 1; j <= n; j++) {
//            while (true) {
 //               for (int q = p + 1; q < min(j, p + magic); q++)
  //                  if (ans[i - 1][q] + cost[j][q] <= ans[i - 1][
    //        }
            while (p + 1 < j && ans[i - 1][p + 1] + cost[j][p + 1] <= ans[i - 1][p] + cost[j][p])
                p++;
            for (int q = p; q < min(j, p + magic); q++)
                ans[i][j] = min(ans[i][j], ans[i - 1][q] + cost[j][q]); 
            for (int q = max(0, j - magic); q < j; q++)
                ans[i][j] = min(ans[i][j], ans[i - 1][q] + cost[j][q]); 
        }
    }

    return ans[k][n];
}

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    std::vector<int> r(n), c(n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &r[i], &c[i]);
    }
    long long ans = take_photos(n, m, k, r, c);
    
    // BEGIN SECRET
    puts("098d134608c94f7413faac591054ee35");
    // END SECRET

    cerr << (clock() + 0.0) / CLOCKS_PER_SEC << endl;
    
    printf("%lld\n", ans);
    return 0;
}
