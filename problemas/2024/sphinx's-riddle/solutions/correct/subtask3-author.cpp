#include <bits/stdc++.h>

#include "sphinx.h"
using namespace std;

#ifdef DEBUG
#define D(x...) fprintf(stderr, x)
#define DA(x) assert(x)
#else
#define D(x...)
#define DA(x)
#endif

const int MAX_N = 250 + 10;

int ans[MAX_N];
vector<int> mask;

int n;

// range is [s, e)
// parity is 0 or 1
int do_query(int s, int e, int parity, int color) {
  fill(mask.begin(), mask.end(), n);

  int ss = s;
  if ((s & 1) == parity) {
    ss--;
  }
  int ee = e;
  if ((e & 1) == parity) {
    ee--;
  }
  D("querying [%d, %d) with:", s, e);
  for (int i = max(0, ss); i <= min(n - 1, ee); i++) {
    if ((i & 1) == parity) {
      mask[i] = -1;
    } else {
      mask[i] = color;
    }
    D(" %d", mask[i]);
  }
  int if_none = 1;
  for (int i = 1; i < n; i++) {
    if (mask[i - 1] != mask[i]) {
      if_none++;
    }
  }

  int res = perform_experiment(mask);
  int delta = if_none - res;
  D(" -> delta=%d [result %d, %d if none]\n", delta, res, if_none);

  return delta;
}

// invariant: delta > 0
// finds all of color in this range (for this parity), and marks them in ans
void go(int s, int e, int parity, int color, int delta) {
  D("go([%d, %d), parity=%d, color=%d, delta=%d)\n", s, e, parity, color,
    delta);
  DA(delta > 0);

  // Handle 0 or 1 elements in range.
  int ss = s;
  if ((s & 1) != parity) {
    ss++;
  }
  if (ss >= e) return;
  if (ss + 2 >= e) {
    ans[ss] = color;
    D("! identified %d as %d\n", ss, color);
    return;
  }

  int mid = (s + e) / 2;

  // TODO: optimize dr call.
  int dl = do_query(s, mid, parity, color);
  /*
  int dr = do_query(mid, e, parity, color);
  DA(dl + dr == delta);
  */
  int dr = delta - dl;

  if (dl) {
    go(s, mid, parity, color, dl);
  }
  if (dr) {
    go(mid, e, parity, color, dr);
  }
}

vector<int> find_colours(int _n, vector<int> /*a*/, vector<int> /*b*/) {
  n = _n;
  mask.resize(n);

  for (int c = 0; c < n; c++) {
    for (int z = 0; z < 2; z++) {
      int delta = do_query(0, n, z, c);
      if (delta) {
        go(0, n, z, c, delta);
      }
    }
  }

  return vector<int>(ans, ans + n);
}
