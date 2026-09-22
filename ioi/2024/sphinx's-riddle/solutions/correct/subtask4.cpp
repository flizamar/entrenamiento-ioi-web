#include "sphinx.h"

using namespace std;

vector<int> find_colours(int N, vector<int> /*X*/, vector<int> /*Y*/) {
  vector<vector<int>> comps;
  comps.push_back({0});
  vector<int> ord(N);
  for (int i = 1; i < N; ++i) {
    ord.assign(N, -1);
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
        for (int u : comps[c]) ord[u] = -1;
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

  int n = comps.size();
  vector<int> comp_factions(n, -1);
  for (int f = 0; f < N; ++f) {
    ord.assign(N, f);
    ord[0] = -1;
    if (perform_experiment(ord) == 1) {
      comp_factions[0] = f;
      break;
    }
  }
  for (int f = 0; f < N; ++f) {
    ord.assign(N, -1);
    for (int u : comps[0]) ord[u] = f;
    if (perform_experiment(ord) == n) continue;

    int lo = 1, hi = n;
    while (lo + 1 < hi) {
      int mid = (lo + hi) / 2;
      ord.assign(N, f);
      for (int c = mid; c < hi; ++c) {
        for (int u : comps[c]) ord[u] = -1;
      }
      int expected = hi - mid + 1;
      if (perform_experiment(ord) < expected) {
        lo = mid;
      } else {
        hi = mid;
      }
    }
    comp_factions[lo] = f;
  }

  vector<int> F(N, -1);
  for (int c = 0; c < n; ++c) {
    for (int u : comps[c]) {
      F[u] = comp_factions[c];
    }
  }
  return F;
}
