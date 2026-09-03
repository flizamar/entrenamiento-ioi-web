#include <queue>

#include "sphinx.h"

using namespace std;

vector<int> find_colours(int N, vector<int> X, vector<int> Y) {
  vector<vector<int>> e;
  e.assign(N, {});
  int M = X.size();
  for (int i = 0; i < M; ++i) {
    vector<int> ord(N, N);
    ord[X[i]] = ord[Y[i]] = -1;
    vector<int> ordchk(N, N);
    ordchk[X[i]] = 0, ordchk[Y[i]] = 1;
    if (perform_experiment(ord) < perform_experiment(ordchk)) {
      e[X[i]].push_back(Y[i]);
      e[Y[i]].push_back(X[i]);
    }
  }

  vector<int> F(N, -1);
  int c = 0;
  queue<int> q;
  for (int i = 0; i < N; ++i) {
    if (F[i] >= 0) continue;
    F[i] = c;
    q.push(i);
    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      for (int nxt : e[cur]) {
        if (F[nxt] == -1) {
          F[nxt] = c;
          q.push(nxt);
        }
      }
    }
    ++c;
  }
  return F;
}
