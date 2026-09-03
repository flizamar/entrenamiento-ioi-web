#include "tree.h"

#include <vector>

using namespace std;

#define forn(i,n) for(int i=0;i<int(n);i++)
#define forsn(i,s,n) for(int i=int(s);i<int(n);i++)
#define dforn(i,n) for(int i=int(n)-1;i>=0;i--)
#define dforsn(i,s,n) for(int i=int(n)-1;i>=int(s);i--)

#define SIZE(c) int((c).size())

const int INF = 1000000000;

vector<long long> sumCapL, sumCapR;
long long leafWeights;
int N;

void init(vector<int> P, vector<int> W) {
    N = SIZE(P);
    vector<int> minNonLeafInSubtree(N, INF);
    vector<int> leavesInSubtree(N, 0);
    vector<bool> leaf(N, true);
    leafWeights = 0;
    dforn(i,N)
    {
        if (leaf[i])
        {
            leafWeights += W[i];
            leavesInSubtree[i]++;
        }
        else
            minNonLeafInSubtree[i] = min(minNonLeafInSubtree[i], W[i]);
        if (i > 0)
        {
            leaf[P[i]] = false;
            minNonLeafInSubtree[P[i]] = min(minNonLeafInSubtree[P[i]], minNonLeafInSubtree[i]);
            leavesInSubtree[P[i]] += leavesInSubtree[i];
        }
    }
    vector<long long> capByLeafCount(N+1, 0);
    forn(i,N)
    if (!leaf[i])
    {
        int parentMin = i == 0 ? 0 : minNonLeafInSubtree[P[i]];
        capByLeafCount[leavesInSubtree[i]] += minNonLeafInSubtree[i] - parentMin;
    }
    sumCapL = sumCapR = vector<long long>(N+2, 0);
    dforn(i,N+1)
    {
        sumCapR[i] = sumCapR[i+1] - capByLeafCount[i];
        sumCapL[i] = sumCapL[i+1] + i * capByLeafCount[i];
    }
}

long long query(int L, int R) {
    int k = min(N+1, (R+L-1)/L);
    return leafWeights * L + sumCapL[k] * L + sumCapR[k] * R;
}
