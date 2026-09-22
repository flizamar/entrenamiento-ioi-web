#include <stdlib.h>
#define max(x, y) ((x) > (y) ? (x) : (y))

const int INF = (int) 1e9;

#define N 1234567

typedef struct {
  int a;
  int b;
  int c;
} st;

int p[N], id[N];
st e[N], ee[N];

int dsu_get(int u) {
  return (u == p[u]) ? u : (p[u] = dsu_get(p[u]));
}

int dsu_union(int u, int v) {
  u = dsu_get(u);
  v = dsu_get(v);
  p[u] = v;
  return (u != v);
}

int comp(void const *i, void const *j) {
  st *x = e + *(int*)i;
  st *y = e + *(int*)j;
  if (x->a != y->a) return x->a - y->a;
  if (x->b != y->b) return x->b - y->b;
  return x->c - y->c;
}

long long plan_roller_coaster(int n, int *s, int *t) {
  int len = 0;
  for (int i = 0; i < n; i++) {
    e[len++] = (st) {s[i], 1, i};
    e[len++] = (st) {t[i], -1, i};
  }
  e[len++] = (st) {INF, 1, n};
  e[len++] = (st) {1, -1, n};
  n++;
  for (int i = 0; i < n; ++i) {
    p[i] = i;
  }
  for (int i = 0; i < len; i++) {
    id[i] = i;
  }
  qsort(id, len, sizeof(int), comp);
  int cn = 0;
  long long res = 0;
  for (int i = 0, delta = 0; i + 1 < len; ++i) {
    delta += e[id[i]].b;
    res += max(0, delta) * (long long) (e[id[i + 1]].a - e[id[i]].a);
    if ((e[id[i + 1]].a == e[id[i]].a) || (delta != 0))
      dsu_union(e[id[i]].c, e[id[i + 1]].c);
    ee[cn++] = (st) {e[id[i + 1]].a - e[id[i]].a, e[id[i]].c, e[id[i + 1]].c};
  }
  for (int i = 0; i < cn; i++) e[i] = (st) {ee[i].a, ee[i].b, ee[i].c};
  for (int i = 0; i < cn; i++) id[i] = i;
  qsort(id, cn, sizeof(int), comp);
  for (int i = 0; i < cn; ++i)
    if (dsu_union(e[id[i]].b, e[id[i]].c))
      res += e[id[i]].a;
  return res;
}

//BEGIN CUT
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int main() {
  int n, i, *s, *t;
  assert(1 == scanf("%d", &n));
  s = (int *) malloc(n * sizeof(int));
  t = (int *) malloc(n * sizeof(int));
  int need_answer;
  assert(1 == scanf("%d", &need_answer));
  for (i = 0; i < n; ++i)
    assert(2 == scanf("%d %d", &s[i], &t[i]));
  long long answer = plan_roller_coaster(n, s, t);
  
  // BEGIN SECRET
  puts("14e047d7a2907b9034950b074822b302");
  // END SECRET
  
  printf("%lld\n", answer);
  free(s);
  free(t);
  return 0;
}
//END CUT
