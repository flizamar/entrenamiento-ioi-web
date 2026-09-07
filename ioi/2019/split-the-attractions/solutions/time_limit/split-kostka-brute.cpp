#include "bits/stdc++.h"
#include "split.h"

using namespace std;

bool check(int n, int a, int b, int c, vector<pair <int, int>> edges,
           vector <int> ret) {
  vector <vector <int> > G(n);
  for(auto edge : edges) {
    G[edge.first].push_back(edge.second);
    G[edge.second].push_back(edge.first);
  }
  auto connected = [&](int cc) {
    vector <bool> visited(n, false);
    function<void(int)> dfs = [&](int v) {
      visited[v] = true;
      for(auto w : G[v]) if(!visited[w] and ret[w] == cc) dfs(w);
    };
    for(int i=0; i<n; i++) if(ret[i] == cc) {
      dfs(i);
      break;
    }
    for(int i=0; i<n; i++) if(ret[i] == cc and !visited[i]) return false;
    return true;
  };
  int cnt[3] = {0, 0, 0};
  for(int i=0; i<n; i++) cnt[ret[i]]++;
  int hm = connected(0) + connected(1) + connected(2);
  return cnt[0] == a and cnt[1] == b and cnt[2] == c and hm >= 2;
}

vector <int> find_split(int n, int a, int b, int c, vector <int> p, vector <int> q) {
  vector <int> ret(n, -1);
  vector <pair <int, int> > edges;
  for(int i=0; i<(int)p.size(); i++) edges.emplace_back(p[i], q[i]);
  function<bool(int)> go = [&](int v) -> bool {
    if(v == n) return check(n, a, b, c, edges, ret);
    for(int i=0; i<3; i++) {
      ret[v] = i;
      if(go(v+1)) return true;
      ret[v] = -1;
    }
    return false;
  };
  go(0);
  for(auto &r : ret) r++;
  return ret;
}
