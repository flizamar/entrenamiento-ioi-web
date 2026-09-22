#include <algorithm>
#include <queue>

#include "sphinx.h"

using namespace std;

int count_components(int N, vector<vector<int>> &adj, vector<int> &col) {
  int cnt = 0;
  vector<bool> vis(N, false);
  queue<int> q;
  for (int i = 0; i < N; ++i) {
    if (vis[i]) {
      continue;
    }
    ++cnt;
    vis[i] = true;
    q.push(i);
    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      for (int nxt : adj[cur]) {
        if (!vis[nxt] && col[nxt] == col[cur]) {
          vis[nxt] = true;
          q.push(nxt);
        }
      }
    }
  }
  return cnt;
}

vector<int> find_components(int N, vector<vector<int>> &adj) {
  vector<vector<int>> comps = {{0}};
  vector<int> ord(N);
  for (int u = 1; u < N; ++u) {
    ord.assign(N, N);
    int n = comps.size();
    vector<int> col(N, N);
    ord[u] = col[u] = -1;
    for (int i = 0; i < n; ++i) {
      for (int v : comps[i]) {
        ord[v] = -1;
        col[v] = i;
      }
    }
    int expected = count_components(N, adj, col);
    int cnt = expected - perform_experiment(ord);
    if (cnt == 0) {
      comps.push_back({u});
      continue;
    }
    int lo = 0, hi = n;
    vector<int> comps_to_merge;
    while (cnt > 0) {
      while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        ord.assign(N, N);
        col.assign(N, N);
        ord[u] = col[u] = -1;
        for (int i = mid; i < hi; ++i)
          for (int v : comps[i]) {
            ord[v] = -1;
            col[v] = i;
          }
        expected = count_components(N, adj, col);
        if (perform_experiment(ord) < expected) {
          lo = mid;
        } else {
          hi = mid;
        }
      }
      comps_to_merge.push_back(lo);
      lo = 0, --hi;
      --cnt;
    }
    int to = comps_to_merge.back();
    comps_to_merge.pop_back();
    for (int from : comps_to_merge) {
      for (int v : comps[from]) {
        comps[to].push_back(v);
      }
      comps.erase(comps.begin() + from);
    }
    comps[to].push_back(u);
  }

  vector<int> cid(N, -1);
  for (int c = 0; c < (int)comps.size(); ++c) {
    for (int v : comps[c]) cid[v] = c;
  }
  return cid;
}

vector<int> find_colours(int N, vector<int> X, vector<int> Y) {
  vector<vector<int>> adj(N);
  int M = X.size();
  for (int i = 0; i < M; ++i) {
    adj[X[i]].push_back(Y[i]);
    adj[Y[i]].push_back(X[i]);
  }

  auto F = find_components(N, adj);

  return F;
}
