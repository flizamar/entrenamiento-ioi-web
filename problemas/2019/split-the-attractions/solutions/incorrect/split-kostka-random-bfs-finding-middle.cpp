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
  for(int _=0; _<100; _++) {
    int root = rand()%n;
    vector <bool> vis(n);
    vector <int> Q = {root};
    vis[root] = true;
    for(int i=0; i<(int)Q.size(); i++) {
      int v = Q[i];
      for(auto w : G[v]) {
        if(!vis[w] and (int)Q.size()+1 <= b) {
          vis[w] = true;
          Q.push_back(w);
        }
      }
    }
    vector <int> C;
    function <void(int)> dfs = [&](int v) {
      C.push_back(v);
      vis[v] = true;
      for(auto w : G[v]) if(!vis[w]) dfs(w);
    };
    for(int i=0; i<n; i++) {
      if(!vis[i]) {
        dfs(i);
        if((int)C.size() >= a) {
          vector <int> ret(n, sizes[2].second);
          for(auto qq : Q) ret[qq] = sizes[1].second;
          for(int j=0; j<a; j++) ret[C[j]] = sizes[0].second;
          return ret;
        }
        C.clear();
      }
    }
  }
  return vector<int>(n, 0);
}
