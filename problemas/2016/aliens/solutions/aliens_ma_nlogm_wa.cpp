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

struct vt {
    llong x, y;
    int num = -1;
    vt(llong _x, llong _y) {
        x = _x, y = _y;
    }
    vt() {}
    friend vt operator -(vt a, vt b) {
        return vt(a.x - b.x, a.y - b.y);
    }
    friend llong operator ^(vt a, vt b) {
        return a.x * b.y - b.x * a.y;
    }
    friend llong operator *(vt a, vt b) {
        return a.x * b.x + a.y * b.y;
    }
};

long long take_photos(int n, int m, int k, std::vector<int> row, std::vector<int> column) {
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
    llong ans = linf;
    llong a = 0, b = (llong)m * m;
    while (b - a > 1) {
        llong x = (a + b) / 2;
        vector<vt> st;
        int pt = 0;
        llong val = 0;
        for (int i = 0; i <= (int)L.size(); i++) {
            val = 0;
            if (i) {
                int r2 = R[i - 1];
                vt dir(1, r2);
                while (pt + 1 < (int)st.size()) {
                    llong scal1 = st[pt] * dir;
                    llong scal2 = st[pt + 1] * dir;
                    if (scal2 < scal1)
                        pt++;
                    else
                        break;
                }
                val = st[pt] * dir + x + (llong)r2 * r2;
            }
            int r1 = i ? R[i - 1] : -1;
            int l = L[i] - 1;
            vt cur(val + (llong)l * l - sqr(max(0, r1 - l)), -2 * l);
            while (st.size() >= 2 && ((st[(int)st.size() - 1] - st[(int)st.size() - 2]) ^ (cur - st[(int)st.size() - 1])) <= 0)
                st.pop_back();
            cur.num = (i == 0) ? 0 : st[pt].num + 1;
            st.push_back(cur);
            pt = min(pt, (int)st.size() - 1);
        }
        int cnt = st.back().num;
        if (cnt > k)
            a = x;
        else {
            ans = min(ans, val - cnt * x);
            b = x;
        }
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

