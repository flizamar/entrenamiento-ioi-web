#include <bits/stdc++.h>

#include "sphinx.h"
using namespace std;

const int MAX_N = 250 + 10;

const int ACTUAL_COLOR = -1;

using pii = pair<int, int>;

struct ComponentCounter {
  int n;
  vector<vector<int>> adjlist;

  int TT;
  vector<int> seen;

  ComponentCounter(vector<vector<int>> _adjlist)
      : n(_adjlist.size()), adjlist(_adjlist), TT(0), seen(n) {}
  ComponentCounter() : ComponentCounter(vector<vector<int>>()) {}

  /*
   * Marks all vertices in u's connected color component as seen, so long as
   * u's color is constant and known as per proposed_colors.
   *
   * proposed_colors is a list of colors that would be used to query.
   * u is a starting vertex
   */
  // Mark all as seen from u.
  void dfs(vector<int> &proposed_colors, int u) {
    seen[u] = TT;
    for (auto v : adjlist[u]) {
      if (proposed_colors[u] != ACTUAL_COLOR &&
          proposed_colors[u] == proposed_colors[v] && seen[v] != TT) {
        dfs(proposed_colors, v);
      }
    }
  }

  /*
   * Return number of connected color-components among vertices, according to
   * proposed_colors (only among known colors).
   */
  int num_components(vector<int> &proposed_colors, vector<int> &vertices) {
    TT++;
    int ret = 0;
    for (auto u : vertices) {
      if (seen[u] != TT) {
        ret++;
        dfs(proposed_colors, u);
      }
    }
    return ret;
  }
};

struct Groups {
  int n;
  vector<int> groupOf;
  vector<vector<int>> members;

  Groups() : Groups(0) {}
  Groups(int _n) : n(_n), groupOf(_n), members(_n) {
    for (int i = 0; i < n; i++) {
      groupOf[i] = i;
      members[i].push_back(i);
    }
  }

  int group_of(int x) { return groupOf[x]; }

  void join_groups_of(int x, int y) {
    int gx = group_of(x);
    int gy = group_of(y);

    if (members[gx].size() > members[gy].size()) {
      swap(gx, gy);
    }

    for (auto u : members[gx]) {
      groupOf[u] = gy;
      members[gy].push_back(u);
    }
    members[gx].clear();
  }

  vector<int> non_empty_groups() {
    vector<int> res;
    for (int i = 0; i < n; i++) {
      if (!members[i].empty()) {
        res.push_back(i);
      }
    }
    return res;
  }

  // Moves groups to the front
  void compress() {
    auto non_empty_group_indices = non_empty_groups();
    int upto = 0;
    for (auto g : non_empty_group_indices) {
      for (auto u : members[g]) {
        groupOf[u] = upto;
      }
      upto++;
    }
    for (int i = 0; i < n; i++) {
      members[i].clear();
    }
    for (int i = 0; i < n; i++) {
      members[groupOf[i]].push_back(i);
    }
  }

  /*
   * Facilitate a query on the "grouping" graph.
   *
   * This is equivalent to setting all vertices in a group to the group's color.
   * If group's color is ACTUAL_COLOR, then all vertices in the group will be
   * set to ACTUAL_COLOR. Note that this color must be the same for all vertices
   * in that group, since vertices in the same group belong to the same
   * color-connected component.
   *
   * Returns the number of color-connected components.
   * Note that this is always the same in the grouping graph as in the original
   * graph.
   */
  int queryGroups(vector<int> mask) {
    vector<int> proposed_colors(n);
    for (int i = 0; i < n; i++) {
      proposed_colors[i] = mask[groupOf[i]];
    }
    return perform_experiment(proposed_colors);
  }
};

/*
 * Identifies the color-connected components, but not their exact colors.
 */
struct Collapser {
  ComponentCounter componentCounter;
  int n;
  Groups grouper;

  Collapser(ComponentCounter _componentCounter)
      : componentCounter(_componentCounter),
        n(_componentCounter.n),
        grouper(n) {}

  // query for number of components in groups [s, e] union {cur}
  int queryOnly(vector<int> &groups, int s, int e, int cur) {
    vector<int> proposed_colors(n, n);
    for (int i = s; i < e; i++) {
      for (auto u : grouper.members[groups[i]]) {
        proposed_colors[u] = ACTUAL_COLOR;
      }
    }
    proposed_colors[cur] = ACTUAL_COLOR;

    vector<int> others;
    for (int i = 0; i < n; i++) {
      if (proposed_colors[i] == n) {
        others.push_back(i);
      }
    }
    int others_components =
        componentCounter.num_components(proposed_colors, others);

    auto found = perform_experiment(proposed_colors);
    auto res = found - others_components;
    return res;
  }

  // how many edges are from cur to vertices in groups [s, e) ?
  int extra_edges(vector<int> &groups, int s, int e, int cur) {
    auto if_none = e - s + 1;
    auto components = queryOnly(groups, s, e, cur);
    auto edges = if_none - components;
    return edges;
  }

  // there are num_edges edges from cur to vertices in groups [s,e).
  // find them and unionise with cur.
  void solve(vector<int> &groups, int s, int e, int num_edges, int cur) {
    if (num_edges == 0) return;
    if (e - s == num_edges) {
      for (int i = s; i < e; i++) {
        auto u = grouper.members[groups[i]].front();
        grouper.join_groups_of(cur, u);
      }
      return;
    }

    int mid = (e + s) / 2;

    auto left_edges = extra_edges(groups, s, mid, cur);
    solve(groups, s, mid, left_edges, cur);
    solve(groups, mid, e, num_edges - left_edges, cur);
  }

  void go() {
    for (int i = 1; i < n; i++) {
      auto non_empty_groups = grouper.non_empty_groups();
      vector<int> groups;
      for (auto g : non_empty_groups) {
        if (g < i) {
          groups.push_back(g);
        }
      }
      auto edges = extra_edges(groups, 0, groups.size(), i);
      solve(groups, 0, groups.size(), edges, i);
    }
  }

  Groups find_colors() {
    go();
    return grouper;
  }
};

struct TwoColor {
  vector<vector<int>> adjlist;
  vector<int> mark;

  // Defines a connected graph
  TwoColor(vector<vector<int>> _adjlist)
      : adjlist(_adjlist), mark(adjlist.size(), -1) {}

  void dfs(int u) {
    for (auto v : adjlist[u]) {
      if (mark[v] == -1) {
        mark[v] = mark[u] ^ 1;
        dfs(v);
      }
    }
  }

  // Returns a vector which is a 2-coloring of the graph, starting at 0.
  vector<int> two_color() {
    mark[0] = 0;
    dfs(0);
    return mark;
  }
};

// solution to subtask where graph is a coloring
namespace coloring {
Groups grouper;
ComponentCounter componentCounter;
vector<int> proposed_colors;

int known_colors[MAX_N];

// Find all of a particular color in subject, and write it to known_colors, and
// return it. Invariant: at least one of color in subject. Invariant:
// proposed_colors of all of test is color, proposed_colors of all of subject is
// ACTUAL_COLOR (or the actual color). here is number of components for
// proposed_colors
vector<int> find_of_color(int color, vector<int> &subject, vector<int> &test,
                          int here) {
  vector<int> ret;
  if (subject.size() <= 1) {
    for (auto u : subject) {
      known_colors[u] = color;
      ret.push_back(u);
    }
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
  int if_none =
      left.size() + componentCounter.num_components(proposed_colors, test);
  auto actual = grouper.queryGroups(proposed_colors);

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
    test.push_back(u);
    if (known_colors[u] == color) {
      proposed_colors[u] = color;
    }
  }
  auto test_components = componentCounter.num_components(proposed_colors, test);
  int if_none_right = right.size() + test_components;

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

  return ret;
}

vector<int> find_colors(Groups _grouper, ComponentCounter _componentCounter,
                        int num_colors) {
  grouper = _grouper;
  componentCounter = _componentCounter;
  int n = componentCounter.n;
  for (int i = 0; i < n; i++) {
    proposed_colors.push_back(ACTUAL_COLOR);
    known_colors[i] = -1;
  }

  if (grouper.non_empty_groups().size() == 1) {
    for (int f = 0; f < num_colors; ++f) {
      vector<int> ord(num_colors, f);
      ord[0] = -1;
      if (perform_experiment(ord) == 1) return {f};
    }
  }

  vector<int> parts[2];
  int comp[2];

  TwoColor twoColor(componentCounter.adjlist);
  auto marks = twoColor.two_color();
  for (int i = 0; i < n; i++) {
    parts[marks[i]].push_back(i);
  }

  for (int z = 0; z < 2; z++) {
    for (auto u : parts[z]) {
      proposed_colors[u] = 0;
    }
    comp[z] = componentCounter.num_components(proposed_colors, parts[z]);
    for (auto u : parts[z]) {
      proposed_colors[u] = ACTUAL_COLOR;
    }
  }

  for (int c = 0; c < num_colors; c++) {
    for (int z = 0; z < 2; z++) {
      for (auto u : parts[z ^ 1]) {
        proposed_colors[u] = c;
      }
      int if_none = parts[z].size() + comp[z ^ 1];
      auto init = grouper.queryGroups(proposed_colors);

      if (init < if_none) {
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
  vector<vector<int>> adjlist(n);
  int m = a.size();
  for (int i = 0; i < m; i++) {
    auto u = a[i];
    auto v = b[i];
    adjlist[u].push_back(v);
    adjlist[v].push_back(u);
  }
  ComponentCounter componentCounter(adjlist);

  // Returned solution has the property that every color component has a
  // distinct value. We use this for grouping.
  Collapser collapser(componentCounter);
  auto collapsed_groups = collapser.find_colors();
  collapsed_groups.compress();

  int collapsedNodes = collapsed_groups.non_empty_groups().size();
  vector<vector<int>> collapsedAdjlist(collapsedNodes);
  for (int u = 0; u < collapsedNodes; u++) {
    unordered_set<int> neighbors;
    for (auto uMember : collapsed_groups.members[u]) {
      for (auto vMember : adjlist[uMember]) {
        auto v = collapsed_groups.group_of(vMember);
        if (u != v) {
          neighbors.insert(v);
        }
      }
    }
    for (auto v : neighbors) {
      collapsedAdjlist[u].push_back(v);
    }
  }
  ComponentCounter collapsedCounter(collapsedAdjlist);

  auto groupedColors =
      coloring::find_colors(collapsed_groups, collapsedCounter, n);
  vector<int> known_colors;
  for (int i = 0; i < n; i++) {
    known_colors.push_back(groupedColors[collapsed_groups.group_of(i)]);
  }

  return known_colors;
}
