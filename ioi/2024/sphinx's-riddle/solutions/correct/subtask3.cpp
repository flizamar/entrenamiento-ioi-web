#include "sphinx.h"

using namespace std;

void solve(int N, vector<int> &spies, vector<int> &ans) {
  if (spies.empty()) return;

  int n = spies.size();
  vector<int> ord(N);
  for (int f = 0; f < N; ++f) {
    ord.assign(N, f);
    for (int u : spies) ord[u] = -1;
    int cnt = (2 * n + 1 - perform_experiment(ord)) / 2;

    int lo = 0, hi = n;
    while (cnt > 0) {
      while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        ord.assign(N, f);
        for (int i = mid; i < hi; ++i) ord[spies[i]] = -1;
        int expected = 2 * (hi - mid) + 1;
        if (perform_experiment(ord) < expected) {
          lo = mid;
        } else {
          hi = mid;
        }
      }
      ans[spies[lo]] = f;
      lo = 0, --hi;
      --cnt;
    }
  }
}

vector<int> find_colours(int N, vector<int> /*X*/, vector<int> /*Y*/) {
  vector<int> F(N, -1);
  vector<int> ord(N);
  for (int f = 0; f < N; ++f) {
    ord.assign(N, f);
    ord[0] = -1;
    if (perform_experiment(ord) == 1) {
      F[0] = f;
      break;
    }
  }
  for (int f = 0; f < N; ++f) {
    ord.assign(N, f);
    ord[N - 1] = -1;
    if (perform_experiment(ord) == 1) {
      F[N - 1] = f;
      break;
    }
  }
  vector<int> spies;
  for (int i = 1; i + 1 < N; i += 2) spies.push_back(i);
  solve(N, spies, F);

  spies.clear();
  for (int i = 2; i + 1 < N; i += 2) spies.push_back(i);
  solve(N, spies, F);

  return F;
}
