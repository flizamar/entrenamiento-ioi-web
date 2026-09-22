#include "nile.h"
#include <vector>
using namespace std;

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    long long total = 0;
    int n = W.size(), mn = INT_MAX;
    for (int i = 0; i < n; i++) {
        total += B[i];
        mn = min(mn, A[i] - B[i]);
    }
    return vector<long long>(E.size(), n & 1 ? total + mn : total);
}