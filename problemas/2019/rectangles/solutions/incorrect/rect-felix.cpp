#include "rect.h"

using namespace std;

#define MAXD 2501

vector<vector<int> > H;
vector<pair<int, int> > nextR[MAXD];
vector<pair<int, int> > nextB[MAXD];
int col[MAXD];
int R, C;

int count_rectangles_bottom(int r, int c, int r2, int c2) {
  for (int cc = c; cc <= c2; cc++) {
    int row = r;
    int next = nextB[cc][row].first;
    if (next <= 0 || nextB[cc][row].second >= H[r-1][cc]) return 0;

    while (next <= r2) {
      row = next;
      auto t =  nextB[cc][next];
      next = t.first;
      if (next <= 0 || t.second >= H[r-1][cc]) return 0;
    }
    if (next != r2 + 1) return 0;
  }
  return 1;
}

int count_rectangles_right(int r, int c, int r2) {
  int cnt = 0;
  int rightBorder = 0;
  for (int rr = r; rr <= r2; rr++) col[rr] = c;
  while (true) {
    bool allRowOk = true;
    for (int rr = r; rr <= r2; rr++) {
      int next = nextR[rr][col[rr]].first;
      if (next <= 0 || nextR[rr][col[rr]].second >= H[rr][c-1]) return cnt;

      while (rightBorder > next) {
        col[rr] = next;
        auto t = nextR[rr][next];
        next = t.first;
        if (next <= 0 || t.second >= H[rr][c-1]) return cnt;

        allRowOk = false;
      }
      if (rightBorder < next) {
        rightBorder = next;
        allRowOk = false;
      }
    }
    if (allRowOk) {
      cnt += count_rectangles_bottom(r, c, r2, rightBorder - 1);
      rightBorder++;
    }
  }
}

vector<pair<int, int> > preprocess(vector<int> &arr) {
  int mx = 0;
  int n = arr.size();
  vector<int> stk {n - 1};
  vector<pair<int, int> > idxAndMax(n);
  for (int r = n - 2; r > 0; r--) {
    mx = max(mx, arr[r]);
    while (!stk.empty() && arr[stk.back()] <= arr[r]) {
      mx = max(mx, arr[stk.back()]);
      stk.pop_back();
    }
    if (!stk.empty()) {
      idxAndMax[r] = make_pair(stk.back(), mx);
    }
    stk.push_back(r);
    mx = 0;
  }
  return idxAndMax;
}

long long count_rectangles(vector<vector<int> > _H) {
  H = _H;
  R = H.size();
  C = H[0].size();

  for (int r = 1; r + 1 < R; r++) {
    vector<int> arr(C);
    for (int c = 0; c < C; c++) arr[c] = H[r][c];
    nextR[r] = preprocess(arr);
  }

  for (int c = 1; c + 1 < C; c++) {
    vector<int> arr(R);
    for (int r = 0; r < R; r++) arr[r] = H[r][c];
    nextB[c] = preprocess(arr);
  }

  long long cnt = 0;
  for (int r = 1; r + 1 < R; r++) {
    for (int c = 1; c + 1 < C; c++) {
      for (int r2 = r; r2 < R; ) {
        int next = nextB[c][r2].first;
        if (next <= 0 || nextB[c][r2].second >= H[r-1][c]) break;
        r2 = next;
        cnt += count_rectangles_right(r, c, r2-1);
      }
    }
  }

  return cnt;
}
