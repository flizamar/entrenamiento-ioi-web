#include <assert.h>

void reverse(int *begin, int *end) {
  --end;
  while (begin < end) {
    int t = *begin;
    *begin = *end;
    *end = t;
    ++begin;
    --end;
  }
}

#define N 222222
#define K 111

int a[N], ans[N], bs[N], ws[N], dp[2][N][K], sumb[N], sumw[N];

void solve_puzzle(int n, char *s, int k, int *c, char *result) {
  int cb = 0;
  int cw = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] == 'X') {
      bs[cb++] = i + 1;
    }
    if (s[i] == '_') {
      ws[cw++] = i + 1;
    }
  }
  n += 2;
  for (int i = 0; i < n; i++) ans[i] = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < n; j++) {
      for (int e = 0; e <= k; e++) {
        dp[i][j][e] = 0;
      }
    }
  }
  for (int it = 0; it < 3; it++) {
    for (int i = 0; i < n; i++) sumb[i] = sumw[i] = 0;
    for (int i = 0; i < cb; i++) sumb[bs[i]]++;
    for (int i = 0; i < cw; i++) sumw[ws[i]]++;
    for (int i = 1; i < n; i++) {
      sumb[i] += sumb[i - 1];
      sumw[i] += sumw[i - 1];
    }
    if (it == 2) break;
    dp[it][0][0] = 1;
    for (int i = 1; i < n; i++) {
      for (int j = 0; j <= k; j++) {
        dp[it][i][j] |= dp[it][i - 1][j] && sumb[i] - sumb[i - 1] == 0;
        dp[it][i][j] |= j > 0 && i > c[j - 1] && sumw[i - 1] - sumw[i - c[j - 1] - 1] == 0 && sumb[i] - sumb[i - 1] == 0 && dp[it][i - c[j - 1] - 1][j - 1];
      }
    }
    reverse(c, c + k);
    reverse(bs, bs + cb);
    reverse(ws, ws + cw);
    for (int i = 0; i < cb; i++) bs[i] = n - bs[i] - 1;
    for (int i = 0; i < cw; i++) ws[i] = n - ws[i] - 1;
  }
  for (int i = 0; i < n - i - 1; i++) {
    for (int j = 0; j <= k; j++) {
      int t = dp[1][i][j];
      dp[1][i][j] = dp[1][n - i - 1][j];
      dp[1][n - i - 1][j] = t;
    }
  }
  for (int i = 1; i < n - 1; i++) {
    for (int j = 0; j <= k; j++) {
      if (dp[0][i][j] && dp[1][i][k - j]) {
        ans[i - 1] |= 1;
      }
    }
  }
  for (int i = 0; i < n; i++) a[i] = 0;
  for (int i = 1; i < n; i++) {
    for (int j = 0; j <= k; j++) {
      if (j > 0 && i > c[j - 1] && sumw[i - 1] - sumw[i - c[j - 1] - 1] == 0 && sumb[i] - sumb[i - 1] == 0 && dp[0][i - c[j - 1] - 1][j - 1] && dp[1][i][k - j]) {
        a[i - c[j - 1]]++;
        a[i]--;
      }
    }
  }
  for (int i = 1; i < n; i++) {
    a[i] += a[i - 1];
    if (a[i] > 0) {
      ans[i - 1] |= 2;
    }
  }
  for (int i = 0; i < n - 2; i++) {
    if (ans[i] == 1) result[i] = '_';
    else if (ans[i] == 2) result[i] = 'X';
    else if (ans[i] == 3) result[i] = '?';
    else assert(0);
  }
}

// BEGIN CUT
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define S_MAX_LEN (200 * 1000)
char s[S_MAX_LEN + 1];

int main() {
    scanf("%s", s);
    int n = strlen(s);
    int k;
    scanf("%d", &k);
    int* c = (int*)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        scanf("%d", &c[i]);
    }
    char* result = (char*)malloc((n + 1) * sizeof(char));
    solve_puzzle(n, s, k, c, result);
    result[n] = 0;

    // BEGIN SECRET
    puts("098d134608c94f7413faac591054ee35");
    // END SECRET
    
    printf("%s\n", result);
    return 0;
}
// END CUT
