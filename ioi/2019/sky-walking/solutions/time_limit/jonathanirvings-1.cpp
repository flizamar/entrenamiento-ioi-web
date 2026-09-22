#include "walk.h"

#include <bits/stdc++.h>
using namespace std;

namespace testcaseCheck {

const int SUBTASK = 1;

struct FenwickTree
{
  vector<int> arr;
  
  FenwickTree(int n) {
    arr.resize(n + 1, 0);
  }
  
  void update(int x,int y) {
    for (int i = x + 1; i < arr.size(); i += (i & -i)) {
      arr[i] += y;
    }
  }

  int query(int x) {
    int ret = 0;
    for (int i = x + 1; i > 0; i -= (i & -i)) {
      ret += arr[i];
    }
    return ret;
  }
};

void run(std::vector<int> x, std::vector<int> h, std::vector<int> l, std::vector<int> r, std::vector<int> y, int s, int g) {
  assert(1 <= SUBTASK && SUBTASK <= 5);

  int n = x.size();
  assert(h.size() == n);

  int m = l.size();
  assert(r.size() == m);
  assert(y.size() == m);

  assert(1 <= n && n <= 100000);
  assert(1 <= m && m <= 100000);
  
  assert(0 <= x[0]);
  for (int i = 0; i < n - 1; ++i) {
    assert(x[i] < x[i + 1]);
  }
  assert(x[n - 1] <= 1000000000);

  for (int i = 0; i < n; ++i) {
    assert(1 <= h[i] && h[i] <= 1000000000);
  }

  for (int i = 0; i < m; ++i) {
    assert(0 <= l[i] && l[i] < r[i] && r[i] < n);
  }

  for (int i = 0; i < m; ++i) {
    assert(1 <= y[i] && y[i] <= min(h[l[i]], h[r[i]]));
  }

  assert(0 <= s && s <= n - 1);
  assert(0 <= g && g <= n - 1);
  assert(s != g);

  map<int, vector<pair<int, int>>> heightToBridges;
  for (int i = 0; i < m; ++i) {
    heightToBridges[y[i]].push_back(make_pair(l[i], r[i]));
  }
  for (pair<int, vector<pair<int, int>>> heightAndBridges : heightToBridges) {
    vector<pair<int, int>> bridges = heightAndBridges.second;
    sort(bridges.begin(), bridges.end());
    for (int i = 1; i < bridges.size(); ++i) {
      assert(bridges[i - 1].second <= bridges[i].first);
    }
  }

  if (SUBTASK == 1) {
    assert(n <= 50 && m <= 50);
  }

  if (SUBTASK == 2) {
    FenwickTree buildingTree(n);
    
    vector<int> heights;
    map<int, vector<int>> heightToBuildings;
    map<int, vector<int>> heightToBridges;

    for (int i = 0; i < n; ++i) {
      heights.push_back(h[i]);
      heightToBuildings[h[i]].push_back(i);
    }
    for (int i = 0; i < m; ++i) {
      heights.push_back(y[i]); 
      heightToBridges[y[i]].push_back(i);
    }

    sort(heights.begin(), heights.end());
    heights.erase(unique(heights.begin(), heights.end()), heights.end());
    reverse(heights.begin(), heights.end());

    for (int currentHeight : heights) {
      for (int building : heightToBuildings[currentHeight]) {
        buildingTree.update(building, 1);
      }
      for (int bridge : heightToBridges[currentHeight]) {
        int intersection = buildingTree.query(r[bridge]) - buildingTree.query(l[bridge] - 1);
        assert(intersection <= 10);
      }
    }
  }

  if (SUBTASK == 3) {
    assert(s == 0);
    assert(g == n - 1);
    for (int i = 0; i < n - 1; ++i) {
      assert(h[i] == h[i + 1]);
    }
  }

  if (SUBTASK == 4) {
    assert(s == 0);
    assert(g == n - 1);
  }
}


}  // namespace testcaseCheck

long long min_distance(std::vector<int> x, std::vector<int> h, std::vector<int> l, std::vector<int> r, std::vector<int> y, int s, int g) {
  // testcaseCheck::run(x, h, l, r, y, s, g);

  int n = x.size();
  int m = l.size();

  map<pair<int, int>, vector<pair<int, int>>> adj;
  for (int i = 0; i < m; ++i) {
    int last = l[i];
    for (int j = l[i] + 1; j <= r[i]; ++j) {
      if (h[j] >= y[i]) {
        adj[make_pair(x[last], y[i])].push_back(make_pair(x[j], y[i]));
        adj[make_pair(x[j], y[i])].push_back(make_pair(x[last], y[i]));
        last = j;
      }
    }
  }

  vector<pair<int, int>> positions;
  for (auto x : adj) {
    positions.push_back(x.first);
  }
  for (int i = 0; i < n; ++i) {
    positions.push_back(make_pair(x[i], 0));
  }
  sort(positions.begin(), positions.end());
  for (int i = 0; i < positions.size() - 1; ++i) {
    if (positions[i].first == positions[i + 1].first) {
      adj[positions[i]].push_back(positions[i + 1]);
      adj[positions[i + 1]].push_back(positions[i]);
    }
  }

  priority_queue<pair<long long, pair<int, int>>> pq;
  set<pair<int, int>> visited;
  pq.push(make_pair(0, make_pair(x[s], 0)));
  while (!pq.empty()) {
    pair<int, int> now = pq.top().second;
    long long dist = -pq.top().first;
    pq.pop();

    if (visited.count(now)) {
      continue;
    }
    visited.insert(now);
    if (now == make_pair(x[g], 0)) {
      return dist;
    }

    for (pair<int, int> nxt : adj[now]) {
      int d = abs(nxt.first - now.first) + abs(nxt.second - now.second);
      pq.push(make_pair(-dist - d, nxt));
    }
  }

  return -1;
}
