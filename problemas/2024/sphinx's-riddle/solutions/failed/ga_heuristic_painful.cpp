#include <algorithm>
#include <chrono>
#include <random>

#include "sphinx.h"

using namespace std;

const int MIN_DEG = 25;

int N;
vector<vector<bool>> edges;
vector<int> deg;
default_random_engine r;
vector<vector<bool>> possible_colors;

int count_components(const vector<int> &coloring) {
  int res = 0;
  vector<int> done(N);
  for (int i = 0; i < N; i++) {
    if (done[i]) continue;
    res++;
    done[i] = true;
    vector<int> stack = {i};
    while (stack.size()) {
      int j = stack.back();
      stack.pop_back();
      for (int k = 0; k < N; k++) {
        if (edges[j][k] && coloring[j] == coloring[k] && !done[k]) {
          done[k] = true;
          stack.push_back(k);
        }
      }
    }
  }
  return res;
}

int find_single(int i) {
  vector<int> colors(N);
  iota(colors.begin(), colors.end(), 0);
  shuffle(colors.begin(), colors.end(), r);
  int start = 0;
  int len = N;
  while (len > 1) {
    vector<int> v(N, N);
    v[i] = -1;
    int k = 0;
    for (int j = 0; j < N && k < len / 2; j++) {
      if (edges[i][j]) {
        v[j] = colors[start + k++];
      }
    }
    if (perform_experiment(v) == count_components(v)) {
      start += k;
      len -= k;
    } else {
      len = k;
    }
  }
  return colors[start];
}

vector<int> find_indep(vector<int> vertices) {
  vector<int> res0;
  shuffle(vertices.begin(), vertices.end(), r);
  vector<int> res;
  for (int i : vertices) {
    bool good = true;
    for (int j : res) {
      if (edges[i][j]) {
        good = false;
      }
    }
    if (good) {
      res.push_back(i);
    }
  }
  return res;
}

int find_in_indep(const vector<int> &indep, int i) {
  vector<int> v0(N, i);
  for (int j : indep) {
    v0[j] = -1;
  }
  if (perform_experiment(v0) == count_components(v0)) {
    for (int j : indep) {
      possible_colors[j][i] = false;
    }
    return -1;
  }
  int start = 0;
  int len = indep.size();
  while (len > 1) {
    vector<int> v(N, i);
    int k = len / 2;
    for (int j = 0; j < k; j++) {
      v[indep[start + j]] = -1;
    }
    if (perform_experiment(v) == count_components(v)) {
      for (int j = start; j < start + k; j++) {
        possible_colors[indep[j]][i] = false;
      }
      start += k;
      len -= k;
    } else {
      len = k;
    }
  }
  return indep[start];
}

std::vector<int> find_colours(int N, std::vector<int> X, std::vector<int> Y) {
  r.seed(chrono::system_clock::now().time_since_epoch().count());
  ::N = N;
  int M = X.size();
  edges.resize(N, vector<bool>(N));
  possible_colors.resize(N, vector<bool>(N, true));
  deg.resize(N);
  for (int i = 0; i < M; i++) {
    edges[X[i]][Y[i]] = edges[Y[i]][X[i]] = true;
    deg[X[i]]++;
    deg[Y[i]]++;
  }
  vector<int> res(N, -1);
  for (int i = 0; i < N; i++) {
    if (deg[i] >= MIN_DEG) {
      res[i] = find_single(i);
    }
  }
  while (true) {
    vector<int> remaining;
    for (int i = 0; i < N; i++) {
      if (res[i] == -1) {
        remaining.push_back(i);
      }
    }
    if (!remaining.size()) {
      break;
    }
    vector<int> indep = find_indep(remaining);
    int best_color = -1;
    int best_count = 0;
    for (int i = 0; i < N; i++) {
      int count = 0;
      for (int j : indep) {
        if (possible_colors[j][i]) {
          count++;
        }
      }
      if (count > best_count) {
        best_count = count;
        best_color = i;
      }
    }
    vector<int> indep2;
    for (int j : indep) {
      if (possible_colors[j][best_color]) {
        indep2.push_back(j);
      }
    }
    int c = find_in_indep(indep2, best_color);
    if (c != -1) {
      res[c] = best_color;
    }
  }
  return res;
}
