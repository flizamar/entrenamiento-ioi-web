#include <random>

#include "sphinx.h"

using namespace std;

int p[250];
bool checked[250];
vector<int> edge[250];
vector<int> ord;
bool reached[250];

int where(int x) {
  if (p[x] < 0) return x;
  return (p[x] = where(p[x]));
}

void dfs(int x) {
  reached[x] = true;
  for (int i : edge[x]) {
    if (!reached[i] && ord[x] == ord[i]) dfs(i);
  }
}

int expected(int N) {
  for (int i = 0; i < N; i++) reached[i] = false;

  int sum = 0;
  for (int i = 0; i < N; i++) {
    if (ord[i] == -1) {
      sum++;
    } else if (!reached[i]) {
      sum++;
      dfs(i);
    }
  }

  return sum;
}

vector<int> find_colours(int N, vector<int> X, vector<int> Y) {
  ord.resize(N);
  for (int i = 0; i < N; i++) p[i] = -1;

  int M = X.size();
  for (int i = 0; i < M; i++) {
    edge[Y[i]].push_back(X[i]);
    edge[X[i]].push_back(Y[i]);
  }

  for (int i = 1; i < N; i++) {
    for (int j = 0; j < N; j++) checked[j] = false;

    for (int j : edge[i]) {
      if (j > i) continue;

      int pp = where(j);
      if (pp == i || checked[pp]) continue;

      for (int k = 0; k < N; k++) ord[k] = N;
      ord[i] = -1;
      ord[j] = -1;

      if (perform_experiment(ord) == expected(N)) {
        checked[pp] = true;
      } else {
        p[pp] = i;
      }
    }
  }

  vector<int> F(N);
  for (int i = 0; i < N; i++) F[i] = where(i);
  return F;
}
