#include <set>

#include "sphinx.h"

using namespace std;

int N;
int p[250];
vector<int> edge[250];
vector<int> ord;
bool reached[250];
set<int> unchecked;
bool keep[250];

int where(int x) {
  if (p[x] < 0) return x;
  return (p[x] = where(p[x]));
}

int color(int x) { return (ord[x] >= 0 ? ord[x] : where(x)); }

void dfs(int x) {
  reached[x] = true;
  for (int i : edge[x]) {
    if (!reached[i] && color(x) == color(i)) dfs(i);
  }
}

int expected() {
  for (int i = 0; i < N; i++) reached[i] = false;

  int sum = 0;
  for (int i = 0; i < N; i++) {
    if (!reached[i]) {
      sum++;
      dfs(i);
    }
  }

  return sum;
}

vector<int> find_colours(int NN, vector<int> X, vector<int> Y) {
  N = NN;
  ord.resize(N);
  for (int i = 0; i < N; i++) p[i] = -1;

  int M = X.size();
  for (int i = 0; i < M; i++) {
    edge[Y[i]].push_back(X[i]);
    edge[X[i]].push_back(Y[i]);
  }

  for (int i = 1; i < N; i++) {
    while (true) {
      for (int j = 0; j <= i; j++) ord[j] = -1;
      for (int j = i + 1; j < N; j++) ord[j] = N;
      if (perform_experiment(ord) == expected()) break;

      unchecked.clear();
      for (int j = 0; j < i; j++) {
        if (where(j) != i) unchecked.insert(where(j));
      }

      vector<int> vec(unchecked.size());
      int pos = 0;
      for (int j : unchecked) vec[pos++] = j;

      int a = 0, b = pos - 1;
      while (a != b) {
        int half = (a + b) / 2;
        for (int j = 0; j < N; j++) keep[j] = false;
        for (int j = a; j <= half; j++) keep[vec[j]] = true;
        for (int j = 0; j < i; j++) {
          if (keep[where(j)]) {
            ord[j] = -1;
          } else {
            ord[j] = N;
          }
        }
        ord[i] = -1;
        for (int j = i + 1; j < N; j++) ord[j] = N;

        if (perform_experiment(ord) == expected()) {
          a = half + 1;
        } else {
          b = half;
        }
      }
      p[where(vec[a])] = i;
    }
  }

  vector<int> F(N);
  for (int i = 0; i < N; i++) F[i] = where(i);
  return F;
}
