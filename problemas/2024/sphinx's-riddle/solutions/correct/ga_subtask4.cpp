#include "sphinx.h"

using namespace std;

std::vector<int> find_colours(int N, std::vector<int> X, std::vector<int> Y) {
  vector<int> res(N);
  int M = X.size();
  for (int i = 0; i < N; i++) {
    int l = 0;
    int u = N;
    while (l + 1 < u) {
      vector<int> v(N, l);
      v[i] = -1;
      int m = (l + u) / 2;
      for (int k = 0; k < m - l; k++) {
        v[k + (k >= i)] = l + k;
      }
      int n = perform_experiment(v);
      (n == m - l ? u : l) = m;
    }
    res[i] = l;
  }
  return res;
}
