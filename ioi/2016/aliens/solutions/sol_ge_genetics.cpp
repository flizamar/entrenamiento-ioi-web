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
const int generation = 5000;
const int iters = 100;

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

vector <int> genPureRandom(int n, int k) {
    set <int> active;
    active.insert(0);

    while ((int) active.size() < k) {
        active.insert(genRandInt(0, n - 1));
    }

    return vector <int>(active.begin(), active.end());
}

vector <int> mutate(int n, vector <int> ans) {
    int p = genRandInt(0, (int) ans.size() - 1);
    
    if (genRandInt(0, 1)) {
        if (ans[p] <= p)
            return ans;
        ans[p]--;
        for (int i = p - 1; i > 0; i--) {
            if (ans[i + 1] > ans[i])
                break;
            ans[i]--;
        }
    } else {
        if ((int) ans.size() - ans[p] <= n - p)
            return ans;
        ans[p]++;
        for (int i = p + 1; i < (int) ans.size(); i++) {
            if (ans[i - 1] < ans[i])
                break;
            ans[i]++;
        }
    }

    return ans; 
}

vector <int> cross(int k, const vector <int>& a, const vector <int>& b) {
    int ps = genRandInt(0, k - 1);    
    
    vector <int> active;
    active.insert(active.begin(), a.begin(), a.begin() + ps);
    active.insert(active.begin(), b.begin() + ps, b.end());

    set <int> tmp(active.begin(), active.end());
    while ((int) tmp.size() < k)
        tmp.insert(genRandInt(0, k - 1));

    return vector <int>(tmp.begin(), tmp.end());
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

    cerr << "here " << segs.size() << endl;

    n = (int) segs.size();
    k = min(n, k);

    vector <pair <long long, vector <int> > > curgen;
    for (int i = 0; i < generation; i++) {
        curgen.pb(mp(0ll, genPureRandom(n, k)));
        curgen.back().fs = get_value(curgen.back().sc);
    }

    sort(curgen.begin(), curgen.end());
    while ((clock() + 0.0) / CLOCKS_PER_SEC < 1.9) {
        
        for (int j = 0; j < generation; j++) {
            curgen.pb(mp(0ll, mutate(n, curgen[genRandInt(0, (int) curgen.size() - 1)].sc)));
            curgen.back().fs = get_value(curgen.back().sc); 
        }
        for (int j = 0; j < generation; j++) {
            curgen.pb(mp(0ll, cross(k, curgen[genRandInt(0, (int) curgen.size() - 1)].sc, curgen[genRandInt(0, (int) curgen.size() - 1)].sc)));
            curgen.back().fs = get_value(curgen.back().sc);
        }
        
        /*
        for (int i = 0; i < generation; i++) {
            curgen.pb(mp(0ll, genPureRandom(n, k)));
            curgen.back().fs = get_value(curgen.back().sc);
        }
        */

        sort(curgen.begin(), curgen.end());

        cerr << curgen[0].fs << endl;

        while ((int) curgen.size() > generation)
            curgen.pop_back(); 
    }

    cerr << "segs" << endl;
    for (int i = 0; i < (int) segs.size(); i++)
        cerr << segs[i].fs << ' ' << segs[i].sc << endl;

    cerr << "ans" << endl;

    for (int i = 0; i < (int) curgen[0].sc.size(); i++)
        cerr << curgen[0].sc[i] << endl;

    return curgen[0].fs;
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
    
    printf("%lld\n", ans);
    return 0;
}
