#include <cstdio>
#include <algorithm>
#include <vector>
#include <cassert>
#include <map>
using namespace std;

typedef long long llong;
const llong linf = 1e13;
const int inf = 1e9;

llong sqr(int x) {
    return 1ll * x * x;
}

const int MAGIC = 300;

long long take_photos(int n, int m, int k, vector<int> r, vector<int> c) {
    map<int, int> M;
    for (int i = 0; i < n; i++) {
        int x = r[i], y = c[i];
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
    vector<vector<int>> mnL(n + 1, vector<int>(n + 1, inf));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j <= n; j++)
            mnL[i][j] = min(mnL[i][j - 1], L[j - 1]);
    }
    vector<llong> D;
    vector<llong> nD;
    D.assign(n + 1, linf);
    D[0] = 0;
    nD = D;
    llong ans = linf;
    vector<vector<llong>> add(n + 1, vector<llong>(n + 1, 0));
    for (int nj = 1; nj <= n; nj++) {
        for (int j = 0; j < nj; j++) {
            int r1 = j ? R[j - 1] : -1;
            int r2 = R[nj - 1];
            int l = mnL[j][nj];
            add[nj][j] = sqr(r2 - l + 1) - sqr(max(0, r1 - l + 1));
        }
    }

    for (int i = 0; i < k; i++) {
        for (int nj = 1; nj <= n; nj++) {
            nD[nj] = linf;
            for (int j = max(0, nj * i / (i + 1) - MAGIC); j < min(nj, nj * i / (i + 1) + MAGIC); j++) {
                llong nv = D[j] + add[nj][j];
                if (nD[nj] > nv)
                    nD[nj] = nv;
            }
        }
        D.swap(nD);
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
