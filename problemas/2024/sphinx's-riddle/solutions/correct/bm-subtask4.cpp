#include "sphinx.h"

using namespace std;

vector<int> find_colours(int N, vector<int> /*X*/, vector<int> /*Y*/) {
  vector<int> F(N);
  vector<int> ord(N);
  for (int i = 0; i < N; i++) {
    int a = 0, b = N - 1;
    while (a != b) {
      int pos = a, half = (a + b) / 2;
      for (int j = 0; j < N; j++) {
        if (i == j) {
          ord[j] = -1;
        } else if (pos <= half) {
          ord[j] = pos++;
        } else {
          ord[j] = N;
        }
      }
      int cc = perform_experiment(ord);
      if (cc == half - a + 1 + 1 + (N > 2 ? 1 : 0)) {
        a = half + 1;
      } else {
        b = half;
      }
    }
    F[i] = a;
  }
  return F;
}
