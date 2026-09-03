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

bool convex(pair<llong, int> a, pair<llong, int> b, pair<llong, int> c) {
    llong dx1 = b.first - a.first, dy1 = b.second - a.second;
    llong dx2 = c.first - b.first, dy2 = c.second - b.second;
    return dx1 * dy2 - dy1 * dx2 > 0;
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
    D.assign(n + 1, linf);
    D[0] = 0;
    nD = D;
    llong ans = linf;
    vector<pair<llong, int>> st;
    st.reserve(n);
    for (int i = 0; i < k; i++) {
        nD.assign(n + 1, linf);
        st.clear();
        nD[0] = 0;
        int pt = 0;
        for (int j = 0; j <= n; j++) {
            if (j) {
                int r2 = R[j - 1];
                while (pt + 1 < (int)st.size()) {
                    llong scal1 = st[pt].first + (llong)st[pt].second * r2;
                    llong scal2 = st[pt + 1].first + (llong)st[pt + 1].second * r2;
                    if (scal1 > scal2)
                        pt++;
                    else
                        break;
                }
                llong val = st[pt].first + (llong)st[pt].second * r2;
                llong cnst = (llong)r2 * r2;
                nD[j] = cnst + val;
            }

            int r1 = j ? R[j - 1] : -1;
            int l = L[j] - 1;
            pair<llong, int> cur(D[j] + (llong)l * l - sqr(max(0, r1 - l)), -2 * l);
            while (st.size() >= 2 && !convex(st[(int)st.size() - 2], st[(int)st.size() - 1], cur))
                st.pop_back();
            st.push_back(cur);
            pt = min(pt, (int)st.size() - 1);
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
