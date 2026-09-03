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
    vector<pair<int, int>> V;
    for (int i = 0; i < n; i++) {
        V.emplace_back(L[i], R[i]);
    }
    long long ans = sqr(V[0].second - V[0].first + 1);
    for (int i = 1; i < n; i++) {
        ans += sqr(V[i].second - V[i].first + 1) - sqr(max(0, V[i - 1].second - V[i].first + 1));
    }
    if (k >= n) {
        return ans;
    }
    for (int i = 0; i < n - k; i++) {
        int mnpos = -1;
        llong mn = linf;
        for (int j = 0; j + 1 < (int)V.size(); j++) {
            llong cur = 2ll * (V[j + 1].second - V[j].second) * (V[j + 1].first - V[j].first);
            if (cur < mn)
                mn = cur, mnpos = j;
        }
        assert(mnpos != -1);
        ans += mn;
        V[mnpos].second = V[mnpos + 1].second;
        V.erase(V.begin() + mnpos + 1);
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
