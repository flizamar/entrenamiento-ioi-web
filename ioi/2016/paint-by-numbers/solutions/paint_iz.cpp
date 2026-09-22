#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

using namespace std;

//std::string solve_puzzle(int n, std::vector<int> C, std::vector<int> B, std::vector<int> W) {
std::string solve_puzzle(std::string S, std::vector<int> C) {
    int n = S.length();
    vector<int> B, W;
    for (int i = 0; i < n; i++) {
        if (S[i] == 'X') {
            B.push_back(i);
        }
        if (S[i] == '_') {
            W.push_back(i);
        }
    }
    n += 2;
    for (int i = 0; i < (int)B.size(); i++) B[i]++;
    for (int i = 0; i < (int)W.size(); i++) W[i]++;
   
    int k = C.size();
    vector<char> ans(n, 0);
    vector<vector<vector<int> > > dp(2, vector<vector<int> >(n, vector<int>(k + 1, 0)));
    vector<int> sumB(n), sumW(n);
    for (int it = 0; it < 3; it++) {
        sumB.assign(n, 0);
        sumW.assign(n, 0);
        for (int x : B) sumB[x]++;
        for (int x : W) sumW[x]++;
        for (int i = 1; i < n; i++) {
            sumB[i] += sumB[i - 1];
            sumW[i] += sumW[i - 1];
        }
        if (it == 2) break;

        dp[it][0][0] = 1;
        for (int i = 1; i < n; i++) {
            for (int j = 0; j <= k; j++) {
                dp[it][i][j] |= dp[it][i - 1][j] && (sumB[i] - sumB[i - 1] == 0);
                dp[it][i][j] |= j > 0 && i > C[j - 1] && sumW[i - 1] - sumW[i - C[j - 1] - 1] == 0 && sumB[i] - sumB[i - 1] == 0 && dp[it][i - C[j - 1] - 1][j - 1];
            }
        }

        reverse(C.begin(), C.end());
        reverse(B.begin(), B.end());
        reverse(W.begin(), W.end());
        for (int i = 0; i < (int)B.size(); i++) B[i] = n - B[i] - 1;
        for (int i = 0; i < (int)W.size(); i++) W[i] = n - W[i] - 1;
    }
    reverse(dp[1].begin(), dp[1].end());
    for (int i = 1; i < n - 1; i++) {
        for (int j = 0; j <= k; j++) {
            if (dp[0][i][j] && dp[1][i][k - j]) {
                ans[i - 1] |= 1;
            }
        }
    }
    vector<int> a(n);
    for (int i = 1; i < n; i++) {
        for (int j = 0; j <= k; j++) {
            if (j > 0 && i > C[j - 1] && sumW[i - 1] - sumW[i - C[j - 1] - 1] == 0 && sumB[i] - sumB[i - 1] == 0 && dp[0][i - C[j - 1] - 1][j - 1] && dp[1][i][k - j]) {
                a[i - C[j - 1]]++;
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
    string res = "";
    for (int i = 0; i < n - 2; i++) {
        if (ans[i] == 1) res += '_';
        else if (ans[i] == 2) res += 'X';
        else if (ans[i] == 3) res += '?';
        else assert(0);
    }
    return res;
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
