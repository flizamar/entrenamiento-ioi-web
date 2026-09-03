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

using pii = pair<int, int>;

// solution to subtask where you just need to identify the color-connected
// components
namespace collapse {
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

vector<int> find_colors(int _n, int m, vector<int> a, vector<int> b) {
  n = _n;
  for (int i = 0; i < n; i++) {
    proposed_colors.push_back(ACTUAL_COLOR);
  }
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
}  // namespace collapse

namespace grouping {
int n;
int nCollapsed;
int groupOf[MAX_N];
vector<int> members[MAX_N];
vector<int> proposed_colors;

/*
 * Facilitate a query on the "grouping" graph.
 *
 * This is equivalent to setting all vertices in a group to the group's color.
 * If group's color is ACTUAL_COLOR, then all vertices in the group will be set
 * to ACTUAL_COLOR. Note that this color must be the same for all vertices in
 * that group, since vertices in the same group belong to the same
 * color-connected component.
 *
 * Returns the number of color-connected components.
 * Note that this is always the same in the grouping graph as in the original
 * graph.
 */
int queryGroups(vector<int> mask) {
  DA(mask.size() == nCollapsed);

  for (int i = 0; i < n; i++) {
    proposed_colors[i] = mask[groupOf[i]];
  }
  return perform_experiment(proposed_colors);
}
}  // namespace grouping

// solution to subtask where graph is a coloring
namespace coloring {
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
  auto actual = grouping::queryGroups(proposed_colors);

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
  actual = grouping::queryGroups(proposed_colors);

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

vector<int> find_colors(int _n, int m, vector<int> a, vector<int> b,
                        int num_colors) {
  n = _n;
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

  if (b.size() == 0) {
    for (int f = 0; f < num_colors; ++f) {
      vector<int> ord(num_colors, f);
      ord[0] = -1;
      if (perform_experiment(ord) == 1) return {f};
    }
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

  for (int c = 0; c < num_colors; c++) {
    for (int z = 0; z < 2; z++) {
      for (int i = 0; i < n; i++) {
        DA(proposed_colors[i] == ACTUAL_COLOR);
      }

      for (auto u : parts[z ^ 1]) {
        proposed_colors[u] = c;
      }
      int if_none = parts[z].size() + comp[z ^ 1];
      auto init = grouping::queryGroups(proposed_colors);
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
}  // namespace coloring

vector<int> find_colours(int n, vector<int> a, vector<int> b) {
  int m = a.size();
  // Returned solution has the property that every color component has a
  // distinct value. We use this for grouping.
  auto collapsed_colors = collapse::find_colors(n, m, a, b);

  grouping::n = n;
  for (int i = 0; i < n; i++) {
    grouping::proposed_colors.push_back(ACTUAL_COLOR);
  }

  grouping::nCollapsed = 0;
  unordered_map<int, int> remap;
  D("remapped:\n");
  for (int i = 0; i < n; i++) {
    auto c = collapsed_colors[i];
    if (remap.count(c) == 0) {
      remap[c] = grouping::nCollapsed;
      grouping::nCollapsed++;
    }
    grouping::groupOf[i] = remap[c];
    grouping::members[remap[c]].push_back(i);
    D("%d ", remap[c]);
  }
  D("\n");

  set<pii> edgesCollapsed;
  for (int u = 0; u < grouping::nCollapsed; u++) {
    for (auto uMember : grouping::members[u]) {
      for (auto vMember : collapse::adjlist[uMember]) {
        auto v = grouping::groupOf[vMember];
        if (u != v && edgesCollapsed.count(make_pair(u, v)) == 0 &&
            edgesCollapsed.count(make_pair(v, u)) == 0) {
          edgesCollapsed.insert(make_pair(u, v));
        }
      }
    }
  }

  int mCollapsed = edgesCollapsed.size();
  vector<int> aCollapsed;
  vector<int> bCollapsed;
  for (auto e : edgesCollapsed) {
    aCollapsed.push_back(e.first);
    bCollapsed.push_back(e.second);
    D("collapsed edge from %d to %d\n", e.first, e.second);
  }

  auto groupedColors = coloring::find_colors(grouping::nCollapsed, mCollapsed,
                                             aCollapsed, bCollapsed, n);
  /*D("grouped colors:\n");
  for (auto g: groupedColors) {
      D("%d ", g);
  }
  D("\n");*/

  vector<int> known_colors;
  for (int i = 0; i < n; i++) {
    known_colors.push_back(groupedColors[grouping::groupOf[i]]);
  }

  return known_colors;
}
