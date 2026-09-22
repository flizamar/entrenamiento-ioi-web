#include <bits/stdc++.h>

#include "sphinx.h"
using namespace std;

#ifdef DEBUG
#define D(x...) fprintf(stderr, x)
#define DA(x) assert(x)
#else
#define D(x...)
#define DA(x)
#endif

const int MAX_N = 250 + 10;

const int ACTUAL_COLOR = -1;

int n;
vector<int> adjlist[MAX_N];
vector<int> proposed_colors;

namespace components {
int TT;
int seen[MAX_N];

// Mark all as seen from u.
void dfs(int u) {
  seen[u] = TT;
  for (auto v : adjlist[u]) {
    if (proposed_colors[u] != ACTUAL_COLOR &&
        proposed_colors[u] == proposed_colors[v] && seen[v] != TT) {
      dfs(v);
    }
  }
}

// Return number of components in a vector
int num_components(vector<int> &vertices) {
  TT++;
  int ret = 0;
  for (auto u : vertices) {
    if (seen[u] != TT) {
      ret++;
      dfs(u);
    }
  }
  return ret;
}
}  // namespace components

namespace forest {
int ugroup[MAX_N];
vector<int> members[MAX_N];

vector<int> get_groups(int limit) {
  vector<int> res;
  for (int i = 0; i < limit; i++) {
    if (!members[i].empty()) {
      res.push_back(i);
    }
  }
  return res;
}

int get(int x) { return ugroup[x]; }

void join(int x, int y) {
  int gx = get(x);
  int gy = get(y);

  if (members[gx].size() > members[gy].size()) {
    swap(gx, gy);
  }

  for (auto u : members[gx]) {
    ugroup[u] = gy;
    members[gy].push_back(u);
  }
  members[gx].clear();
}

// query for number of components in groups [s, e] union {cur}
int queryOnly(vector<int> &groups, int s, int e, int cur) {
  D("queryOnly\n");
  fill(proposed_colors.begin(), proposed_colors.end(), n);
  for (int i = s; i < e; i++) {
    for (auto u : members[groups[i]]) {
      D("%d ", u);
      proposed_colors[u] = ACTUAL_COLOR;
    }
  }
  proposed_colors[cur] = ACTUAL_COLOR;
  D("%d\n", cur);

  vector<int> others;
  for (int i = 0; i < n; i++) {
    if (proposed_colors[i] == n) {
      others.push_back(i);
    }
  }
  int others_components = components::num_components(others);

  auto found = perform_experiment(proposed_colors);
  auto res = found - others_components;
  D("queryOnly %d: found %d, others components = %d\n", res, found,
    others_components);
  return res;
}

// how many edges are from cur to vertices in groups [s, e) ?
int extra_edges(vector<int> &groups, int s, int e, int cur) {
  auto if_none = e - s + 1;
  auto components = queryOnly(groups, s, e, cur);
  auto edges = if_none - components;
  D("extra_edges=%d for [%d, %d) with cur=%d: if_none=%d, components=%d\n",
    edges, s, e, cur, if_none, components);
  return edges;
}

// there are num_edges edges from cur to vertices in groups [s,e).
// find them and unionise with cur.
void solve(vector<int> &groups, int s, int e, int num_edges, int cur) {
  D("solve([");
  for (int i = s; i < e; i++) {
    D("%d ", groups[i]);
  }
  D("], num_edges=%d, cur=%d)\n", num_edges, cur);

  DA(num_edges <= e - s);
  if (num_edges == 0) return;
  if (e - s == num_edges) {
    for (int i = s; i < e; i++) {
      auto u = members[groups[i]].front();
      D("! identified that %d and %d are in the same color component\n", cur,
        u);
      join(cur, u);
    }
    return;
  }

  DA(e - s > 1);
  int mid = (e + s) / 2;

  auto left_edges = extra_edges(groups, s, mid, cur);
  DA(0 <= left_edges && left_edges <= num_edges);
  solve(groups, s, mid, left_edges, cur);
  solve(groups, mid, e, num_edges - left_edges, cur);
}

void go() {
  for (int i = 0; i < n; i++) {
    ugroup[i] = i;
    members[i].push_back(i);
  }

  for (int i = 1; i < n; i++) {
    auto groups = get_groups(i);
    D("* i=%d\n", i);
    auto edges = extra_edges(groups, 0, groups.size(), i);
    solve(groups, 0, groups.size(), edges, i);
  }
}
}  // namespace forest

vector<int> find_colours(int _n, vector<int> a, vector<int> b) {
  n = _n;
  for (int i = 0; i < n; i++) {
    proposed_colors.push_back(ACTUAL_COLOR);
  }
  int m = a.size();
  for (int i = 0; i < m; i++) {
    auto u = a[i];
    auto v = b[i];
    adjlist[u].push_back(v);
    adjlist[v].push_back(u);
  }
  forest::go();

  vector<int> ret;
  for (int i = 0; i < n; i++) {
    int g = forest::get(i);
    ret.push_back(g);
  }
  return ret;
}
