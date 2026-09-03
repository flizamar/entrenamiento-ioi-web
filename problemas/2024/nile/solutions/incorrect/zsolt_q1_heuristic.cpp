#include "nile.h"

#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using ll = long long;
using pii = pair<int,int>;

vector<ll> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
    int N = (int)W.size();
    vector<pii> ord(N);
    for (int i = 0; i < N; ++i)
        ord[i] = {W[i], i};
    sort(ord.begin(), ord.end());
    vector<int> w(N), a(N), b(N);
    for (int i = 0; i < N; ++i) {
        int j = ord[i].second;
        w[i] = W[j], a[i] = A[j], b[i] = B[j];
    }
    vector<ll>v;
    for(int curq = 0; curq < E.size(); ++curq)
    {
        int D = E[curq];
        ll ans = 0ll;
        for (int i = 0; i < N;) {
            ans += b[i];
            int j = i+1, worst = a[i] - b[i];
            for (; j < N; ++j) {
                if (w[j] - w[j-1] > D)
                    break;
                ans += b[j];
                worst = min(worst, a[j] - b[j]);
            }
            if ((j - i) % 2 == 1)
                ans += worst;
            i = j;
        }
        v.push_back(ans);
    }
    return v;
}
