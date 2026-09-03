#include "nile.h"
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    vector<long long> costs;
    int n = W.size(), m = E.size();
    vector<tuple<int, int, int>> helper;
    for (int i = 0; i < n; i++)
        helper.push_back({W[i], A[i], B[i]});
    sort(helper.begin(), helper.end());
    for (int i = 0; i < n; i++) {
        W[i] = get<0>(helper[i]);
        A[i] = get<1>(helper[i]);
        B[i] = get<2>(helper[i]);
    }

    for (int i = 0; i < m; i++) {
        long long best = LONG_LONG_MAX;
        for (int msk = 0; msk < 1 << n; msk++) {
            if (__builtin_popcount(msk) & 1)
                continue;

            bool ok = true;
            long long cost = 0;
            for (int j = 0, prev = -1; ok && j < n; j++)
                if (msk >> j & 1)
                    if (~prev) {
                        ok &= abs(W[prev] - W[j]) <= E[i];
                        cost += B[j] + B[prev];
                        prev = -1;
                    }
                    else
                        prev = j;
                else
                    cost += A[j];

            if (ok)
                best = min(best, cost);
        }
        costs.push_back(best);
    }
    return costs;
}