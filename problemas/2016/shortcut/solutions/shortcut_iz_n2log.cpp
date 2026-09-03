#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
//#include "shortcut.h"

using namespace std;

typedef long long ll;

const ll INF = (ll)1e18;
long long find_shortcut(int n, std::vector<int> l, std::vector<int> d, int c) {
    vector<ll> x(n);
    x[0] = 0;
    for (int i = 1; i < n; i++) x[i] = x[i - 1] + l[i - 1];

    ll L = 0, R = INF;
    while (R - L > 1) {
        ll M = ((ll)L + R) / 2;

        ll minSum = -INF, maxSum = INF;
        ll minDif = -INF, maxDif = INF;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                // abs(x[i] - l) + abs(x[j] - r) <= M - c - d[i] - d[j]
                // abs(x[i] - l) <= M - c - d[i] - d[j] - abs(x[j] - r)
                // x[i] - l <= M - c - d[i] - d[j] - abs(x[j] - r)
                // x[i] - l >= -M + c + d[i] + d[j] + abs(x[j] - r)

                // abs(x[j] - r) <= M - c - d[i] - d[j] - x[i] + l
                // x[j] - r <= M - c - d[i] - d[j] - x[i] + l
                // l + r >= -M + c + d[i] + d[j] + x[i] + x[j]
                // x[j] - r >= -M + c + d[i] + d[j] + x[i] - l
                // l - r >= -M + c + d[i] + d[j] + x[i] - x[j]  


                // abs(x[j] - r) <= M - c - d[i] - d[j] + x[i] - l
                // x[j] - r <= M - c - d[i] - d[j] + x[i] - l
                // l - r <= M - c - d[i] - d[j] + x[i] - x[j]
                // x[j] - r >= -M + c + d[i] + d[j] - x[i] + l
                // l + r <= M - c - d[i] - d[j] + x[i] + x[j]

                if (x[j] - x[i] + d[i] + d[j] > M) {
                    minSum = max(minSum, -M + c + d[i] + d[j] + x[i] + x[j]);
                    maxSum = min(maxSum, M - c - d[i] - d[j] + x[i] + x[j]);
                    minDif = max(minDif, -M + c + d[i] + d[j] + x[i] - x[j]);
                    maxDif = min(maxDif, M - c - d[i] - d[j] + x[i] - x[j]);
                }
            }
        }
        bool found = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                ll sum = x[i] + x[j];
                ll dif = x[i] - x[j];
                if (minSum <= sum && sum <= maxSum && minDif <= dif && dif <= maxDif) {
                    found = 1;
                }
            }
        }
        if (found) R = M;
        else L = M;
    }
    return R;
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
