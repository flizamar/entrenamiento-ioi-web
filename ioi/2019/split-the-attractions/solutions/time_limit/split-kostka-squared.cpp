#include "bits/stdc++.h"
#include "split.h"

using namespace std;

bool check(int n, int a, int b, int c, vector<pair <int, int>>& edges,
           vector <int> ret) {
  vector <vector <int> > G(n);
  for(auto edge : edges) {
    G[edge.first].push_back(edge.second);
    G[edge.second].push_back(edge.first);
  }
  auto connected = [&](int c) {
    vector <bool> visited(n, false);
    function<void(int)> dfs = [&](int v) {
      visited[v] = true;
      for(auto w : G[v]) if(!visited[w] and ret[w] == c) dfs(w);
    };
    for(int i=0; i<n; i++) if(ret[i] == c) {
      dfs(i);
      break;
    }
    for(int i=0; i<n; i++) if(ret[i] == c and !visited[i]) return false;
    return true;
  };
  for(int i=0; i<n; i++) ret[i]--;
  int cnt[3] = {0, 0, 0};
  for(int i=0; i<n; i++) cnt[ret[i]]++;
  int hm = connected(0) + connected(1) + connected(2);
  return cnt[0] == a and cnt[1] == b and cnt[2] == c and hm >= 2;
}

vector <int> find_split(int n, int a, int b, int c, vector <int> p, vector <int> q) {
  vector <pair <int, int> > edges;
  for(int i=0; i<(int)p.size(); i++) edges.emplace_back(p[i], q[i]);
  vector <int> old_sizes = {a, b, c};
  vector <pair <int, int> > sizes = {{a,0}, {b,1}, {c,2}};
  sort(sizes.begin(), sizes.end());
  a = sizes[0].first; b = sizes[1].first; c = sizes[2].first;
  vector <int> rep(n), siz(n);
  for(int i=0; i<n; i++) {
    rep[i] = i;
    siz[i] = 1;
  }
  function <int(int)> find = [&](int x) {
    if(rep[x] == x) return x;
    return rep[x] = find(rep[x]);
  };
  auto unia = [&](int aa, int bb) -> bool {
    int fa = find(aa), fb = find(bb);
    if(fa == fb) return false;
    if(siz[fa] < siz[fb]) swap(fa, fb);
    siz[fa] += siz[fb];
    rep[fb] = fa;
    return true;
  };
  vector <vector <int> > G(n);
  vector <pair <int, int> > non_tree;
  for(auto edge : edges) {
    if(unia(edge.first, edge.second)) {
      G[edge.first].push_back(edge.second);
      G[edge.second].push_back(edge.first);
    } else {
      non_tree.push_back(edge);
    }
  }
  vector <bool> vis(n);
  vector <int> subtree(n);
  function<void(int)> dfs1 = [&](int v) {
    vis[v] = true;
    subtree[v] = 1;
    for(auto w : G[v]) if(!vis[w]) {
      dfs1(w);
      subtree[v] += subtree[w];
    }
  };
  dfs1(0);
  vis = vector<bool>(n, false);
  pair <int, int> both_dir = {-1, -1};
  vector <pair <int, int> > dir;
  function<void(int)> dfs2 = [&](int v) {
    vis[v] = true;
    for(auto w : G[v]) if(!vis[w]) {
      int below = subtree[w], above = n - subtree[w];
      if(min(below, above) >= a) {
        if(above < below) both_dir = {v, w};
        else both_dir = {w, v};
      }
      else {
        if(below < a) dir.emplace_back(v, w);
        else if(above < a) dir.emplace_back(w, v);
      }
      dfs2(w);
    }
  };
  dfs2(0);
  auto find_solution = [&](int pa, int pb) -> vector<int> {
    vector <int> ret(n, -1);
    int counter;
    function<void(int, int)> dfs3 = [&](int v, int mark) {
      if(counter == 0) return;
      --counter;
      ret[v] = mark;
      for(auto w : G[v]) if(ret[w] == -1) dfs3(w, mark);
    };
    ret[pa] = sizes[0].second;
    ret[pb] = sizes[a].second;
    counter = a;
    dfs3(pa, sizes[0].second);
    vis[pb] = sizes[1].second;
    counter = b;
    dfs3(pb, sizes[1].second);
    vis[pa] = false;
    for(int i=0; i<n; i++) if(ret[i] == -1) ret[i] = sizes[2].second;
    for(int i=0; i<n; i++) ret[i]++;
    return ret;
  };
  if(both_dir.first != -1) {
    return find_solution(both_dir.first, both_dir.second);
  } else {
    for(auto edge : non_tree) {
      G[edge.first].push_back(edge.second);
      G[edge.second].push_back(edge.first);
    }
    for(int root=0; root<n; root++) {
      vis = vector<bool>(n, false);
      subtree = vector<int>(n, 0);
      dfs1(root);
      vector <pair <int, int> > X;
      for(auto son : G[root]) {
        if(subtree[son] >= a) {
          auto sol = find_solution(son, root);
          if(check(n, old_sizes[0], old_sizes[1], old_sizes[2], edges, sol)) return sol;
          else break;
        }
      }
    }
    return vector<int>(n, 0);
  }
}
