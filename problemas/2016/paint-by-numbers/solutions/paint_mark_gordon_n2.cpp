#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdio>
 
using namespace std;
 
string solve_puzzle(string S, vector<int> C) {
  int M = C.size();
  int N = S.size();
 
  vector<bool> isb(N + 1, false);
  vector<int> cntb(N + 1, 0);
  for (int i = 0; i < N; i++) {
    if (S[i] == 'X') {
      isb[i] = true;
      cntb[i + 1] = 1;
    }
  }
 
  vector<bool> isw(N + 1, false);
  vector<int> cntw(N + 1, 0);
  for (int i = 0; i < N; i++) {
    if (S[i] == '_') {
      isw[i] = true;
      cntw[i + 1] = 1;
    }
  }
 
  for (int i = 1; i <= N; i++) {
    cntb[i] += cntb[i - 1];
    cntw[i] += cntw[i - 1];
  }
 
  vector<vector<bool> > DP_F(N + 1, vector<bool>(M + 1));
 
  DP_F[N][M] = true;
  for (int i = N - 1; i >= 0; i--) {
    for (int j = 0; j <= M; j++) {
      if (!isb[i]) {
        DP_F[i][j] = DP_F[i + 1][j];
      }
      if (j < M && i + C[j] <= N && DP_F[min(N, i + C[j] + 1)][j + 1] &&
            (i + C[j] == N || !isb[i + C[j]]) &&
            cntw[i] == cntw[i + C[j]]) {
        DP_F[i][j] = true;
      }
    }
  }
 
  vector<vector<bool> > DP_R(N + 1, vector<bool>(M + 1));
 
  DP_R[0][0] = true;
  for (int i = 1; i <= N; i++) {
    for (int j = M; j >= 0; j--) {
      if (!isb[i - 1]) {
        DP_R[i][j] = DP_R[i - 1][j];
      }
      if (j > 0 && i - C[j - 1] >= 0 && DP_R[max(0, i - C[j - 1] - 1)][j - 1] &&
            (i - C[j - 1] == 0 || !isb[i - C[j - 1] - 1]) &&
            cntw[i] == cntw[i - C[j - 1]]) {
        DP_R[i][j] = true;
      }
    }
  }
 
  S = "";
  for (int i = 0; i < N; i++) {
    bool canw = false;
    for (int j = 0; j <= M; j++) {
      canw |= !isb[i] && DP_R[i][j] && DP_F[i + 1][j];
    }
     
    bool canb = false;
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < C[j]; k++) {
        if (0 <= i - k && i + C[j] - k <= N) {
          canb |= cntw[i - k] == cntw[i + C[j] - k] &&
                (i - k == 0 || !isb[i - k - 1]) &&
                (i + C[j] - k == N || !isb[i + C[j] - k]) &&
                DP_R[max(0, i - k - 1)][j] &&
                DP_F[min(N, i + C[j] - k + 1)][j + 1];
        }
      }
    }
 
    if (canw && canb) {
      S += '?';
    } else if (canw) {
      S += '_';
    } else if (canb) {
      S += 'X';
    } else {
      S += '*';
    }
  }
 
    return S;
}

// BEGIN CUT
const int S_MAX_LEN = 200 * 1000;
char buf[S_MAX_LEN + 1];

int main() {
    scanf("%s", buf);
    std::string s = buf;
    int c_len;
    scanf("%d", &c_len);
    std::vector<int> c(c_len);
    for (int i = 0; i < c_len; i++) {
        scanf("%d", &c[i]);
    }
    std::string ans = solve_puzzle(s, c);

    // BEGIN SECRET
    puts("098d134608c94f7413faac591054ee35");
    // END SECRET

    printf("%s\n", ans.data());
    return 0;
}
// END CUT
