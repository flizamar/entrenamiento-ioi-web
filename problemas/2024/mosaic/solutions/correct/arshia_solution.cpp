#include "mosaic.h"
#include <numeric>
#include <vector>
using namespace std;

const int S = 10;

vector<long long> mosaic(vector<int> X, vector<int> Y, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
	int N = X.size(), Q = T.size();
	vector<long long> C(Q, 0);

	vector<int> help(N - 1 << 1 | 1, 0);
	copy(Y.rbegin(), Y.rend(), help.begin());
	copy(X.begin() + 1, X.end(), help.begin() + N);

	for (int s = 0; s < S; s++) {
		vector<int> sum(N - s << 1, 0);
		partial_sum(help.begin(), help.end(), sum.begin() + 1);
		for (int i = 0; i < Q; i++)
			if (T[i] <= B[i] && L[i] <= R[i]) {
				if (T[i] == s)
					C[i] += sum[R[i] - T[i] + N - s] - sum[L[i] - T[i] + N - s - 1], T[i]++;
				if (L[i] == s)
					C[i] += sum[L[i] - T[i] + N - s] - sum[L[i] - B[i] + N - s - 1], L[i]++;
			}

		if (s == N - 1)
			return C;
		vector<int> nxt(N - s - 2 << 1 | 1, 0);
		for (int i = 0; i < N - s - 1; i++) {
			nxt[i + N - s - 2] = !help[i + N - s] && !(i ? nxt[i + N - s - 3] : help[N - s - 2]);
			nxt[N - s - 2 - i] = !help[N - s - i - 2] && !(i ? nxt[N - s - 1 - i] : help[N - s]);
		}
		help = nxt;
	}

	vector<long long> sum(N - S << 1, 0);
	partial_sum(help.begin(), help.end(), sum.begin() + 1), partial_sum(sum.begin(), sum.end(), sum.begin());
	for (int i = 0; i < Q; i++)
		if (T[i] <= B[i] && L[i] <= R[i])
			C[i] += sum[R[i] - T[i] + N - S] - sum[L[i] - T[i] + N - S - 1] - sum[R[i] - B[i] + N - S - 1] + (L[i] - B[i] + N - S - 2 >= 0 ? sum[L[i] - B[i] + N - S - 2] : 0);

	return C;
}
