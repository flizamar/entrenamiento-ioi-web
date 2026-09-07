#include <assert.h>
#include <stdlib.h>

#define min(x, y) (x) > (y) ? (y) : (x)
#define max(x, y) (x) < (y) ? (y) : (x)

#define N 1234567

typedef struct {
  int start;
  int end;
} range;

range ranges[N];
long long as[N], bs[N];
int fs[N];
int size, opt;

int comp(void const *a_, void const *b_) {
  range *a = (range *)a_;
  range *b = (range *)b_;
  if (a->start != b->start) return a->start - b->start;
  return b->end - a->end;
}

void clear_cht() {
  size = opt = 0;
}

double intersection(long long a1, long long b1, long long a2, long long b2) {
  return 1.0 * (double) (b2 - b1) / (double) (a1 - a2);
}

int get_max(long v) {
  opt = min(opt, size - 1);
  while (opt + 1 < size) {
    double x = intersection(as[opt], bs[opt], as[opt + 1], bs[opt + 1]);
    if (v > x) {
      opt++;
    } else {
      break;
    }
  }
  return opt;
}

void add_line(long long a, long long b, int bomb) {
  while (size > 1) {
    double x1 = intersection(as[size - 2], bs[size - 2], as[size - 1], bs[size - 1]);
    double x2 = intersection(as[size - 1], bs[size - 1], a, b);
    if (x1 < x2) {
      break;
    } else {
      size--;
    }
  }
  as[size] = a;
  bs[size] = b;
  fs[size] = bomb;
  size++;
}

long long sqr(long long x) {
  return x * x;
}

void getValue(int n, long long c, long long *value_res, long long *bombs_res) {
  clear_cht();
  add_line(-2 * (ranges[0].start - 1), sqr(ranges[0].start - 1), 0);
  for (int i = 1; i <= n; i++) {
    int line = get_max(ranges[i - 1].end);
    long long value = as[line] * ranges[i - 1].end + bs[line] + sqr(ranges[i - 1].end) + c;
    int bombs = fs[line] + 1;
    if (i < n) {
      add_line(-2 * (ranges[i].start - 1), value + sqr(ranges[i].start - 1) - sqr(max(0, ranges[i - 1].end - ranges[i].start + 1)), bombs);
    } else {
      *value_res = value;
      *bombs_res = bombs;
      return;
    }
  }
  assert(0);
}

long long take_photos(int n, int m, int k, int *x, int *y) {
  for (int i = 0; i < n; i++) {
    ranges[i] = (range) {min(x[i], y[i]), max(x[i], y[i])};
  }
  qsort(ranges, n, sizeof(range), comp);
  int maxRight = -1;
  int cn = 0;
  for (int i = 0; i < n; i++) {
    if (ranges[i].end > maxRight) {
      maxRight = ranges[i].end;
      ranges[cn++] = ranges[i];
    }
  }
  n = cn;
  k = min(k, n);
  clear_cht();
  long long left = 0;
  long long right = 1LL * m * m + 1;
  if (right % 2 != 0) {
    ++right;
  }
  while (left < right - 1) {
    long long mid = (left + right) >> 1;
    long long c = mid + 1;
    long long value, bombs;
    getValue(n, c, &value, &bombs);
    if (bombs < k) {
      right = mid;
    } else {
      left = mid;
    }
  }
  long long value1, bombs1, value2, bombs2;
  getValue(n, left + 1, &value1, &bombs1);
  getValue(n, right + 1, &value2, &bombs2);
  long long answer1 = value1 - bombs1 * (left + 1);
  long long answer2 = value2 - bombs2 * (right + 1);
  if (bombs1 == k) {
    return answer1;
  } else if (bombs2 == k) {
    return answer2;
  } else {
    long long diff = answer1 - answer2;
    return answer1 + diff / (bombs1 - bombs2) * (k - bombs1);
  }
}

// BEGIN CUT

#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    int* r = (int*)malloc(n * sizeof(int));
    int* c = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &r[i], &c[i]);
    }
    long long ans = take_photos(n, m, k, r, c);
    
    // BEGIN SECRET
    puts("098d134608c94f7413faac591054ee35");
    // END SECRET
    
    printf("%lld\n", ans);
    return 0;
}

// END CUT
