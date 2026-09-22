#include "bits/stdc++.h"
#include "split.h"

using namespace std;

vector<int> find_split(int n, int a, int b, int c, vector<int> p, vector<int> q) {
  srand(10);
  int m = (int)p.size();
  vector <pair <int, int> > sizes = {{a,1}, {b,2}, {c,3}};
  sort(sizes.begin(), sizes.end());
  a = sizes[0].first; b = sizes[1].first; c = sizes[2].first;
  vector <vector <int> > G(n);
  for(int i=0; i<m; i++) {
    G[p[i]].push_back(q[i]);
    G[q[i]].push_back(p[i]);
  }
  vector <vector <int> > T(n);
  vector <int> subtree(n), par(n);
  function <int(int)> dfs0 = [&](int v) {
    subtree[v] = 1;
    for(auto w : G[v]) if(!subtree[w]) {
      subtree[v] += dfs0(w);
      par[w] = v;
      T[v].push_back(w);
      T[w].push_back(v);
    }
    return subtree[v];
  };
  dfs0(0);
  for(int i=0; i<n; i++) {
    vector <bool> vis(n);
    vector <int> C;
    function <void(int, int, int)> dfs = [&](int v, int limit, int forbidden) {
      if((int)C.size() == limit) return;
      if(forbidden == v) return;
      C.push_back(v);
      vis[v] = true;
      for(auto w : T[v]) if(!vis[w]) dfs(w, limit, forbidden);
    };
    if(subtree[i] >= a and subtree[i] <= n-b) {
      vector <int> ret(n, sizes[2].second);
      dfs(i, a, par[i]);
      for(auto cc : C) ret[cc] = sizes[0].second;
      C.clear();
      dfs(par[i], b, i);
      for(auto cc : C) ret[cc] = sizes[1].second;
      return ret;
    } else if(subtree[i] >= b and subtree[i] <= n-a) {
      vector <int> ret(n, sizes[2].second);
      dfs(i, b, par[i]);
      for(auto cc : C) ret[cc] = sizes[1].second;
      C.clear();
      dfs(par[i], a, i);
      for(auto cc : C) ret[cc] = sizes[0].second;
      return ret;
    }
  }
  return vector<int>(n, 0);
}
