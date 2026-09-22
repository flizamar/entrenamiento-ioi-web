#include "bits/stdc++.h"
#include "split.h"

using namespace std;

vector <int> find_split(int n, int a, int b, int c, vector <int> p, vector <int> q) {
  vector <int> path;
  vector <bool> vis;
  vector <pair <int, int> > edges;
  vector <vector <int> > G(n);
  vis.resize(n);
  for(int i=0; i<(int)p.size(); i++) {
    G[p[i]].push_back(q[i]);
    G[q[i]].push_back(p[i]);
  }
  function<void(int)> dfs = [&](int v) {
    path.push_back(v);
    vis[v] = true;
    for(auto w : G[v]) if(!vis[w]) dfs(w);
  };
  for(int i=0; i<n; i++) {
    if(G[i].size() == 1) {
      dfs(i);
      break;
    }
  }
  if(path.empty()) {
    return vector<int>(n, 0);
  }
  vector <int> ret(n);
  for(int i=0; i<a; i++) {
    ret[path.back()] = 1;
    path.pop_back();
  }
  for(int i=0; i<b; i++) {
    ret[path.back()] = 2;
    path.pop_back();
  }
  for(int i=0; i<c; i++) {
    ret[path.back()] = 3;
    path.pop_back();
  }
  return ret;
}
