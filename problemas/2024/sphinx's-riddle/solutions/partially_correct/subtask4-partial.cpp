#include "sphinx.h"

using namespace std;

vector<int> find_colours(int N, vector<int> /*X*/, vector<int> /*Y*/) {
  vector<vector<int>> comps;
  comps.push_back({0});
  for (int i = 1; i < N; ++i) {
    vector<int> ord(N, -1);
    for (int j = i + 1; j < N; ++j) ord[j] = N;
    if (perform_experiment(ord) == (int)comps.size() + 1 + (i + 1 < N)) {
      comps.push_back({i});
      continue;
    }

    int lo = 0, hi = comps.size();
    while (lo + 1 < hi) {
      int mid = (lo + hi) / 2;
      ord.assign(N, N);
      ord[i] = -1;
      for (int c = mid; c < hi; ++c) {
        for (int u : comps[c]) {
          ord[u] = -1;
        }
      }
      int expected = 2 + hi - mid;
      if (perform_experiment(ord) < expected) {
        lo = mid;
      } else {
        hi = mid;
      }
    }
    comps[lo].push_back(i);
  }

  vector<int> F(N, -1);
  for (int c = 0; c < (int)comps.size(); ++c) {
    for (int u : comps[c]) {
      F[u] = c;
    }
  }
  return F;
}
