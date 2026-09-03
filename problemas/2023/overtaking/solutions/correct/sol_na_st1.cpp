#include "overtaking.h"

using namespace std;
using ll = long long;

int L, N, X, M;
vector<long long> T;
vector<int> W, S;

void init(int L_, int N_, std::vector<long long> T_, std::vector<int> W_, int X_, int M_, std::vector<int> S_)
{
    L = L_;
    N = N_;
    T = T_;
    W = W_;
    X = X_;
    M = M_;
    S = S_;
    return;
}

long long arrival_time(long long Y)
{
    std::vector<long long> prev = {T[0], Y};
    for (int i = 1; i < M; ++i)
    {
        vector<long long> uj = {prev[0] + ll(S[i] - S[i - 1]) * W[0], prev[1] + ll(S[i] - S[i - 1]) * X};
        if ((prev[0] > prev[1] && uj[0] < uj[1]) || (prev[0] < prev[1] && uj[0] > uj[1]))
        {
            long long x = max(uj[0], uj[1]);
            uj[0] = uj[1] = x;
        }
        prev.swap(uj);
    }
    return prev[1];
}