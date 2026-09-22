#include<bits/stdc++.h>
#include "hieroglyphs.h"

using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;


vi lcs(const vi& a, const vi& b) {
	int n = a.size();
	int m = b.size();

	vvi dp(n+1, vi(m+1));
	for (int i=1; i <= n; ++i) {
		for (int j=1; j <= m; ++j) {
			dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
			if (a[i-1] == b[j-1]) dp[i][j] = max(dp[i][j], dp[i-1][j-1]+1);
		}
	}

	vi c;
	int ci = n;
	int cj = m;
	while (ci > 0 && cj > 0) {
		if (a[ci-1] == b[cj-1] && dp[ci][cj] == dp[ci-1][cj-1]+1) {
			c.push_back(a[ci-1]);
			ci--;
			cj--;
		}
		else {
			if (dp[ci][cj] == dp[ci-1][cj]) {
				ci--;
			}
			else {
				cj--;
			}
		}
	}

	reverse(c.begin(), c.end());
	return c;
}

vector<int> ucs(vi a, vi b) {
	vi c = lcs(a, b);

	int n = a.size();
	int m = b.size();
	int l = c.size();

	vvi nxt(max(n, m)+1, vi(l+2, l)); //nxt[x][i] = first k such that c[k] = x and k >= i, l if such k does not exist

	for (int i=0; i < l; ++i) {
		for (int j=0; j <= i; ++j) {
			nxt[c[i]][j] = min(nxt[c[i]][j], i);
		}
	}

	vvi dp(n+1, vi(m+1, -1)); //dp[i][j] = maximum k so that there is a common subseq of a[0..i), b[0..j) which is not a subseq of c[0..k), -1 if no subseq

	for (int i=1; i <= n; ++i) {
		for (int j=1; j <= m; ++j) {
			if (a[i-1] == b[j-1]) {
				dp[i][j] = nxt[a[i-1]][dp[i-1][j-1]+1];
			}
			dp[i][j] = max(dp[i][j], max(dp[i-1][j], dp[i][j-1]));
		}
	}

	if(dp[n][m] == l) return vector<int>({-1});
	return c;
}
