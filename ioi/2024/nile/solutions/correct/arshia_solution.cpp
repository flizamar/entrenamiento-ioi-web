#include "nile.h"
#include <tuple>
#include <vector>
#include <numeric>
#include <climits>
#include <algorithm>
#include <cassert>
using namespace std;

struct DSU {
    vector<int> parent, size;
    vector<int> even, odd;
    long long cost = 0;

    DSU(vector<int> &A, vector<int> &B) : parent(A.size(), -1), size(A.size(), 1), even(A.size()), odd(A.size(), INT_MAX) {
        for (int i = 0; i < A.size(); i++) {
            even[i] = A[i] - B[i];
            cost += even[i];
        }
    }
    int get_par(int x) {
        return ~parent[x] ? parent[x] = get_par(parent[x]) : x;
    }
    void merge(int v) {
        int u = get_par(v - 1);
        for (int x : {u, v})
            if (size[x] & 1)
                cost -= even[x];

        if (size[u] & 1)
            swap(even[v], odd[v]);
        odd[u] = min(odd[u], odd[v]);
        even[u] = min(even[u], even[v]);

        parent[v] = u;
        size[u] += size[v];
        if (size[u] & 1)
            cost += even[u];
    }
    void consider(int v, int w) {
        int u = get_par(v);
        if (v - u & 1) {
            if (size[u] & 1)
                cost -= even[u];
            even[u] = min(even[u], w);
            if (size[u] & 1)
                cost += even[u];
        }
        else
            odd[u] = min(odd[u], w);
    }
};

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    int n = W.size(), q = E.size();
    if (n == 1)
        return vector<long long>(q, A[0]);

    vector<tuple<int, int, int>> helper;
    for (int i = 0; i < n; i++)
        helper.push_back({W[i], A[i], B[i]});
    sort(helper.begin(), helper.end());
    for (int i = 0; i < n; i++)
        tie(W[i], A[i], B[i]) = helper[i];

    vector<int> diff1(n - 1);
    iota(diff1.begin(), diff1.end(), 1);
    sort(diff1.begin(), diff1.end(), [&](int i, int j) {
        return W[i] - W[i - 1] < W[j] - W[j - 1];
    });

    vector<int> diff2(n - 2);
    iota(diff2.begin(), diff2.end(), 1);
    sort(diff2.begin(), diff2.end(), [&](int i, int j) {
        return W[i + 1] - W[i - 1] < W[j + 1] - W[j - 1];
    });

    vector<int> indexes(q);
    iota(indexes.begin(), indexes.end(), 0);
    sort(indexes.begin(), indexes.end(), [&](int i, int j) {
        return E[i] < E[j];
    });

    DSU dsu(A, B);
    int p1 = 0, p2 = 0;
    vector<long long> costs(q);
    long long total = accumulate(B.begin(), B.end(), 0LL);
    for (int index : indexes) {
        while (p1 < n - 1 && W[diff1[p1]] - W[diff1[p1] - 1] <= E[index]) {
            int idx = diff1[p1++];
            dsu.merge(idx);

            for (int p : {idx, idx - 1})
                if (p && p < n - 1 && W[p + 1] - W[p - 1] <= E[index] && dsu.get_par(p + 1) == dsu.get_par(p - 1))
                    dsu.consider(p, A[p] - B[p]);
        }
        while (p2 < n - 2 && W[diff2[p2] + 1] - W[diff2[p2] - 1] <= E[index]) {
            int idx = diff2[p2++];
            if (dsu.get_par(idx + 1) == dsu.get_par(idx - 1))
                dsu.consider(idx, A[idx] - B[idx]);
        }
        costs[index] = total + dsu.cost;
    }

    return costs;
}