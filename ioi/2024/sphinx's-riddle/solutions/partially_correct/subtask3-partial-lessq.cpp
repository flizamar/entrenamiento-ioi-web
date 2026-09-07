#include <queue>

#include "sphinx.h"

using namespace std;

int count_comp(int N, vector<vector<int>> &adj, int u, int v) {
  int cnt = 0;
  vector<bool> vis(N, false);
  queue<int> q;
  for (int i = 0; i < N; ++i) {
    if (i == u || i == v || vis[i]) continue;
    ++cnt;
    vis[i] = true;
    q.push(i);
    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      for (int nxt : adj[cur]) {
        if (nxt == u || nxt == v || vis[nxt]) continue;
        vis[nxt] = true;
        q.push(nxt);
      }
    }
  }
  return cnt;
}

vector<int> find_colours(int N, vector<int> X, vector<int> Y) {
  vector<vector<int>> adj, e;
  adj.assign(N, {}), e.assign(N, {});
  int M = X.size();
  for (int i = 0; i < M; ++i)
    adj[X[i]].push_back(Y[i]), adj[Y[i]].push_back(X[i]);
  for (int i = 0; i < M; ++i) {
    vector<int> ord(N, N);
    ord[X[i]] = ord[Y[i]] = -1;
    int cnt = count_comp(N, adj, X[i], Y[i]);
    if (perform_experiment(ord) == cnt + 1) {
      e[X[i]].push_back(Y[i]);
      e[Y[i]].push_back(X[i]);
    }
  }

  vector<int> F(N, -1);
  int c = 0;
  queue<int> q;
  for (int i = 0; i < N; ++i) {
    if (F[i] >= 0) continue;
    F[i] = c;
    q.push(i);
    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      for (int nxt : e[cur]) {
        if (F[nxt] == -1) {
          F[nxt] = c;
          q.push(nxt);
        }
      }
    }
    ++c;
  }
  return F;
}
