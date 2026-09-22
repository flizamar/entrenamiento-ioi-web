#include "sphinx.h"

using namespace std;

struct interval {
  int start, end, count;
};

int query_interval(int N, int f, int r, int start, int end) {
  vector<int> v(N, f);
  vector<interval> iv;
  int count = 1;
  for (int j = start; j < end; j++) {
    if (j % 2 == r) {
      v[j] = -1;
      count += j == N - 1 ? 1 : 2;
    }
  }
  return (count - perform_experiment(v) + 1) / 2;
}

std::vector<int> find_colours(int N, std::vector<int> X, std::vector<int> Y) {
  int M = X.size();
  vector<int> res(N);
  for (int i = 0; i < N; i++) {
    vector<int> v(N, i);
    v[0] = -1;
    if (perform_experiment(v) == 1) {
      res[0] = i;
      break;
    }
  }
  for (int r = 0; r < 2; r++) {
    for (int i = 0; i < N; i++) {
      vector<interval> intervals;
      intervals.push_back({1, N, query_interval(N, i, r, 1, N)});
      while (intervals.size()) {
        interval iv = intervals.back();
        intervals.pop_back();
        if (iv.count == 0) continue;
        if ((iv.start + r + 3) / 2 == (iv.end + r + 1) / 2) {
          res[(iv.start + r + 1) / 2 * 2 - r] = i;
        } else {
          int mid = (iv.start + iv.end) / 2;
          int count = query_interval(N, i, r, iv.start, mid);
          intervals.push_back({iv.start, mid, count});
          intervals.push_back({mid, iv.end, iv.count - count});
        }
      }
    }
  }
  return res;
}
