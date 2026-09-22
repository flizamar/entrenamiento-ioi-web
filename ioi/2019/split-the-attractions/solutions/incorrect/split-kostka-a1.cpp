#include "bits/stdc++.h"
#include "split.h"

using namespace std;

vector <int> find_split(int n, int a, int b, int c, vector <int> p, vector <int> q) {
  vector <vector <int> > G(n);
  for(int i=0; i<(int)p.size(); i++) {
    G[p[i]].push_back(q[i]);
    G[q[i]].push_back(p[i]);
  }
  int counter = a+b;
  vector <bool> vis(n);
  vector <int> ret(n, 3);
  function<void(int)> dfs = [&](int v) {
    if(counter == 0) return;
    counter--;
    vis[v] = true;
    ret[v] = (counter ? 2 : 1);
    for(auto w : G[v]) {
      if(!vis[w]) dfs(w);
    }
  };
  dfs(0);
  return ret;
}
