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
const int size = 50 * 1050;
const int ssize = 210;
const int iters = 1000 * 1000 * 100;
const long long inf = (long long) 1e18;

vector <pair <int, int> > segs;

random_device rd;
mt19937 rgenerator(rd());

int n_;
    
int genRandInt(int lb, int rb) {
    return lb + rgenerator() % (rb - lb + 1);
}

long long sqr(long long x) {
    return x * x;
}

long long ans[ssize][size];
//long long cost[size][size];

long long cost(int i, int j) {
    if (j == n_)
        return sqr(segs.back().sc - segs[i].fs + 1);
    else
        return sqr(segs[j - 1].sc - segs[i].fs + 1) - sqr(max(0, segs[j - 1].sc - segs[j].fs + 1));
}

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
    n_ = n;
    
    /*
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            cost[j][i] = sqr(segs[j - 1].sc - segs[i].fs + 1) - sqr(max(0, segs[j - 1].sc - segs[j].fs + 1));
        }
        cost[n][i] = sqr(segs.back().sc - segs[i].fs + 1);
    }
    */
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
            bool flag = true;
            while (flag) {
                flag = false;
                for (int q = p + 1; q < min(j, p + magic); q++)
                    if (ans[i - 1][q] + cost(q, j) <= ans[i - 1][p] + cost(p, j)) {
                        p = q;
                        flag = true;
                        break;
                    }

                if (!flag) {
                    int step = max(1, (j - p) / magic);
                    for (int q = p + magic; q < j; q += step)
                        if (ans[i - 1][q] + cost(q, j) <= ans[i - 1][p] + cost(p, j)) {
                            for (int t = max(p + 1, q - magic); t <= q; t++) {
                                if (ans[i - 1][t] + cost(t, j) <= ans[i - 1][p] + cost(p, j)) {
                                    p = t;
                                    flag = true;
                                    break;
                                }
                            }
                            flag = true;
                            break;
                    }
    
                }
            }
            ans[i][j] = min(ans[i - 1][j], ans[i - 1][p] + cost(p, j)); 
            //while (p + 1 < j && ans[i - 1][p + 1] + cost(p + 1, j) <= ans[i - 1][p] + cost(p, j))
            //    p++;
            //for (int q = p; q < min(j, p + magic); q++)
            //    ans[i][j] = min(ans[i][j], ans[i - 1][q] + cost(q, j)); 
            for (int q = max(p + 1, j - magic); q < j; q++) {
                long long h = ans[i - 1][q] + cost(q, j);
                if (h <= ans[i][j]) {
                    ans[i][j] = h;
                    p = q;
                }
//                ans[i][j] = min(ans[i][j], ans[i - 1][q] + cost(q, j)); 
            }
            for (int q = max(0, j - j / i - magic / 2); q < min(0, j - j / i + magic / 2); q++) {
                long long h = ans[i - 1][q] + cost(q, j);
                if (h <= ans[i][j]) {
                    ans[i][j] = h;
                    p = q;
                }
//                ans[i][j] = min(ans[i][j], ans[i - 1][q] + cost(q, j)); 
            }

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
