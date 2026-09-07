//#include "hieroglyphs.h"
#include <iostream>
#include <vector>

using namespace std;

#define forn(i,n) for (int i=0;i<int(n);i++)
#define dforn(i,n) for (int i=int(n)-1;i>=0;i--)
#define SIZE(c) int((c).size())

const int MAX_NUM = 200000;

vector<int> aIndex[MAX_NUM];
vector<int> bIndex[MAX_NUM];

vector<int> lcs(const vector<int> &A, const vector<int> &B, bool maximizing)
{
	const int N = SIZE(A);
	const int M = SIZE(B);
	vector<vector<int>> dp(N+1, vector<int>(M+1));
	forn(j,M+1)
		dp[0][j] = 0;
	forn(i,N+1)
		dp[i][0] = 0;
	dforn(i,N)
	dforn(j,M)
	if (A[i] == B[j])
		dp[i][j] = 1 + dp[i+1][j+1];
	else
		dp[i][j] = max(dp[i][j+1], dp[i+1][j]);
	vector<int> ret;
	int i = 0, j = 0;
	while (dp[i][j] > 0)
	{
		if (A[i] == B[j])
		{
			ret.push_back(A[i]);
			i++;
			j++;
		}
		else if (dp[i+1][j] < dp[i][j+1])
			j++;
		else if (dp[i+1][j] > dp[i][j+1])
			i++;
		else if ((A[i] < B[j]) != maximizing)
			j++;
		else
			i++;
	}
	return ret;
}

vector<int> ucs(vector<int> A, vector<int> B) {
  vector<int> v1 = lcs(A,B,false);
  vector<int> v2 = lcs(A,B,true);
  if (v1 == v2)
	return v1;
  else
    return {-1};
}
