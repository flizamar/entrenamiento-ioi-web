#include <cstdio>
#include <algorithm>
#include <vector>
#include <cassert>
#include <iostream>
#include <map>
using namespace std;

typedef long long llong;
const llong linf = 1e13;
const int inf = 1e9;

llong sqr(int x) {
    return 1ll * x * x;
}

long long take_photos(int n, int /* m */, int k, std::vector<int> row, std::vector<int> column) {
    map<int, int> M;
    for (int i = 0; i < n; i++) {
        int x, y;
        x = row[i];
        y = column[i];
        if (x < y)
            swap(x, y);
        if (!M.count(x))
            M[x] = x;
        M[x] = min(M[x], y);
    }
    vector<int> L, R;
    for (auto pr : M) {
        while (!L.empty() && L.back() >= pr.second)
            L.pop_back(), R.pop_back();
        R.push_back(pr.first);
        L.push_back(pr.second);
    }
    n = L.size();
    assert(is_sorted(R.begin(), R.end()));
    assert(is_sorted(L.begin(), L.end()));
    vector<llong> D;
    vector<llong> nD;
    vector<int> opt;
    vector<int> nopt;
    D.assign(n + 1, linf);
    opt.assign(n + 1, 0);
    D[0] = 0;
    nD = D;
    llong ans = linf;

    // opt[k][n] >= opt[k - 1][n]
    // opt[k][n] <= opt[k][n + 1]
    for (int i = 0; i < k; i++) {
        nD.assign(n + 1, linf);
        nopt.assign(n + 1, 0);
        for (int nj = n; nj >= 0; nj--) {
            for (int j = opt[nj]; j <= ((nj == n) ? n : nopt[nj + 1]); j++) {
                int r1 = j ? R[j - 1] : -1;
                int r2 = R[nj - 1];
                int l = L[j];
                llong nv = D[j] + sqr(r2 - l + 1) - sqr(max(0, r1 - l + 1));
                if (nD[nj] > nv) {
                    nD[nj] = nv;
                    nopt[nj] = j;
                }
            }
        }
        D.swap(nD);
        opt.swap(nopt);
        ans = min(ans, D.back());
    }
    return ans;
}

// BEGIN CUT

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

// END CUT
