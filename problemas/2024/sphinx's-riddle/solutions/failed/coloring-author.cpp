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

int known_colors[MAX_N];

namespace divide {
int mark[MAX_N];

void dfs(int u) {
  for (auto v : adjlist[u]) {
    if (mark[v] == -1) {
      mark[v] = mark[u] ^ 1;
      dfs(v);
    }
  }
}

void go() {
  fill(mark, mark + n, -1);
  mark[0] = 0;
  dfs(0);
}
}  // namespace divide

namespace components {
int TT;
int seen[MAX_N];

// Mark all as seen from u.
void dfs(int u) {
  // D("++ got to %d\n", u);
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
      // D("+ new component %d\n", u);
      ret++;
      dfs(u);
    }
  }
  return ret;
}
}  // namespace components

void print_vec(vector<int> &v) {
  D("[");
  for (auto i = 0; i < (int)v.size(); i++) {
    if (i > 0) {
      D(", ");
    }
    D("%d", v[i]);
  }
  D("]");
}

// Find all of a particular color in subject, and write it to known_colors, and
// return it. Invariant: at least one of color in subject. Invariant:
// proposed_colors of all of test is color, proposed_colors of all of subject is
// ACTUAL_COLOR (or the actual color). here is number of components for
// proposed_colors
vector<int> find_of_color(int color, vector<int> &subject, vector<int> &test,
                          int here) {
  D("find_of_color (color=%d, here=%d):\n", color, here);
  D(" subject: ");
  print_vec(subject);
  D("\n");
  D(" test: ");
  print_vec(test);
  D("\n");

  vector<int> ret;
  if (subject.size() <= 1) {
    for (auto u : subject) {
      known_colors[u] = color;
      ret.push_back(u);
      D("! identified %d as %d\n", u, color);
    }
    DA(!ret.empty());
    return ret;
  }

  auto mid = subject.begin() + (subject.size() / 2);
  auto left = vector<int>(subject.begin(), mid);
  auto right = vector<int>(mid, subject.end());

  // check if any on left first
  for (auto u : right) {
    test.push_back(u);
    proposed_colors[u] = color;
  }
  int if_none = left.size() + components::num_components(test);
  auto actual = perform_experiment(proposed_colors);

  if (actual < if_none) {
    // there's some on the left
    ret = find_of_color(color, left, test, actual);
  }

  for (auto u : right) {
    test.pop_back();
    proposed_colors[u] = ACTUAL_COLOR;
  }

  // check if left accounts for everything
  for (auto u : left) {
    // D("pushing %d [proposed_color=%d]\n", u, proposed_colors[u]);
    test.push_back(u);
    if (known_colors[u] == color) {
      // D("setting %d to %d\n", u, color);
      proposed_colors[u] = color;
    }
  }
  auto test_components = components::num_components(test);
  int if_none_right = right.size() + test_components;
  D("here=%d, if_none_right = %d test_components=%d, test=", here,
    if_none_right, test_components);
  print_vec(test);
  D(" left=");
  print_vec(left);
  D(" right=");
  print_vec(right);
  D(" ret=");
  print_vec(ret);
  D("\n");

  if (here < if_none_right) {
    // there's some on the right
    auto ret_right = find_of_color(color, right, test, here);
    for (auto u : ret_right) {
      ret.push_back(u);
    }
  }
  for (auto u : left) {
    proposed_colors[u] = ACTUAL_COLOR;
    test.pop_back();
  }

  /*
  // check if any on right first
  for (auto u: left) {
      test.push_back(u);
      proposed_colors[u] = color;
  }
  if_none = right.size() + components::num_components(test);
  actual = query(proposed_colors);

  if (actual < if_none) {
      // there's some on the left
      auto ret_right = find_of_color(color, right, test, actual);
      for (auto u: ret_right) {
          ret.push_back(u);
      }
  }

  for (auto u: left) {
      test.pop_back();
      proposed_colors[u] = ACTUAL_COLOR;
  }
  */

  DA(!ret.empty());
  return ret;
}

vector<int> find_colours(int _n, vector<int> a, vector<int> b) {
  n = _n;
  int m = a.size();
  for (int i = 0; i < n; i++) {
    proposed_colors.push_back(ACTUAL_COLOR);
    known_colors[i] = -1;
  }
  for (int i = 0; i < m; i++) {
    auto u = a[i];
    auto v = b[i];
    adjlist[u].push_back(v);
    adjlist[v].push_back(u);
  }

  vector<int> parts[2];
  int comp[2];

  divide::go();
  for (int i = 0; i < n; i++) {
    DA(divide::mark[i] == 0 || divide::mark[i] == 1);
    parts[divide::mark[i]].push_back(i);
  }

  for (int z = 0; z < 2; z++) {
    for (auto u : parts[z]) {
      proposed_colors[u] = 0;
    }
    comp[z] = components::num_components(parts[z]);
    D("comp[%d] = %d\n", z, comp[z]);
    for (auto u : parts[z]) {
      proposed_colors[u] = ACTUAL_COLOR;
    }
  }

  for (int c = 0; c < n; c++) {
    for (int z = 0; z < 2; z++) {
      for (int i = 0; i < n; i++) {
        DA(proposed_colors[i] == ACTUAL_COLOR);
      }

      for (auto u : parts[z ^ 1]) {
        proposed_colors[u] = c;
      }
      int if_none = parts[z].size() + comp[z ^ 1];
      auto init = perform_experiment(proposed_colors);
      D("* trying color=%d, z=%d [if_none=%d, init=%d]\n", c, z, if_none, init);

      if (init < if_none) {
        D("* starting color=%d, z=%d [if_none=%d, init=%d]\n", c, z, if_none,
          init);
        find_of_color(c, parts[z], parts[z ^ 1], init);
      }
      for (auto u : parts[z ^ 1]) {
        proposed_colors[u] = ACTUAL_COLOR;
      }
    }
  }

  return vector<int>(known_colors, known_colors + n);
}
