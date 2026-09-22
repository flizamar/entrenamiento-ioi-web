#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
//#include "shortcut.h"

using namespace std;

typedef long long ll;

const ll INF = (ll)1e18;

ll calc(int l, int r, int n, const vector<ll> &x, const vector<int> &d, int c) {
    ll ans = 0;
    ll left = -INF, right = -INF;
    for (int i = 0; i <= l; i++) {
        ans = max(ans, x[i] + d[i] + left);
        left = max(left, -x[i] + d[i]);
    }
    for (int i = n - 1; i >= r; i--) {
        ans = max(ans, right - x[i] + d[i]);
        right = max(right, x[i] + d[i]);
    }
    ans = max(ans, min(right + left, right + left - x[r] + x[l] + c));
    for (int i = l; i <= r; i++) {
        ans = max(ans, min(x[i] + d[i] + left, x[r] - x[i] + d[i] + c + x[l] + left));
        ans = max(ans, min(right - x[i] + d[i], d[i] + x[i] - x[l] + c + right - x[r]));
    }
    for (int i = l; i < r; i++) {
        for (int j = i + 1; j <= r; j++) {
            ans = max(ans, min(x[j] + d[j] - x[i] + d[i], x[i] - x[l] + d[i] + c + x[r] - x[j] + d[j]));
        }
    }
    if (ans == 15) cerr << l << " " << r << endl;
    return ans;
}

long long find_shortcut(int n, std::vector<int> l, std::vector<int> d, int c) {
    double start = (double)clock() / CLOCKS_PER_SEC;
    vector<ll> x(n);
    x[0] = 0;
    for (int i = 1; i < n; i++) x[i] = x[i - 1] + l[i - 1];

    ll ans = INF;
    for (int len = n; len >= 1; len--) {
        for (int l = 0; l + len <= n; l++) {
//            if (clock() / (double)CLOCKS_PER_SEC - start > 1.5) return ans;
            int r = l + len - 1;
            ans = min(ans, calc(l, r, n, x, d, c));
            if (ans == 50) cerr << l << " " << r << endl;
        }
    }

    return ans;
}

// BEGIN CUT

int main() {
    int n, c;
    scanf("%d%d", &n, &c);	
    std::vector<int> l(n - 1);
    std::vector<int> d(n);
    for (int i = 0; i < n - 1; i++)
        scanf("%d", &l[i]);
    for (int i = 0; i < n; i++)
        scanf("%d", &d[i]);
    
    long long t = find_shortcut(n, l, d, c);

    // BEGIN SECRET
    puts("14e047d7a2907b9034950b074822b302");
    // END SECRET

    printf("%lld\n", t);

    return 0;
}

// END CUT
