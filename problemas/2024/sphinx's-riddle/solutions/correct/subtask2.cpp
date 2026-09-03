#include "sphinx.h"

using namespace std;

vector<int> find_colours(int N, vector<int> /*X*/, vector<int> /*Y*/) {
  vector<int> F(N);
  for (int i = 0; i < N; ++i) {
    for (int c = 0; c < N; ++c) {
      vector<int> ord(N, c);
      ord[i] = -1;
      if (perform_experiment(ord) == 1) {
        F[i] = c;
        break;
      }
    }
  }
  return F;
}
