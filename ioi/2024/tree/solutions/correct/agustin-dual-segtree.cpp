#include "tree.h"

#include <vector>
#include <cassert>

using namespace std;

#define forn(i,n) for(int i=0;i<int(n);i++)
#define forsn(i,s,n) for(int i=int(s);i<int(n);i++)
#define dforn(i,n) for(int i=int(n)-1;i>=0;i--)
#define dforsn(i,s,n) for(int i=int(n)-1;i>=int(s);i--)

#define SIZE(c) int((c).size())

vector<long long> sumCapL, sumCapR;
long long leafWeights;
int N;
vector<int> W;


struct Data
{
    int minIndex; // Index of min in subtree, -1 as neutral element
    int leafCount;
    
    Data() { minIndex = -1; leafCount = 0; }
    Data(int mini, int leaves) { minIndex = mini; leafCount = leaves; }
    
    bool operator ==(const Data &o) const { return minIndex == o.minIndex && leafCount == o.leafCount; }
};

const Data NEUTRAL;
const Data LEAF(-1,1);

Data op(const Data &a, const Data &b)
{
    Data ret;
    ret.leafCount = a.leafCount + b.leafCount;
    if (a.minIndex < 0 || b.minIndex < 0)
        ret.minIndex = a.minIndex + b.minIndex + 1;
    else if (W[a.minIndex] < W[b.minIndex])
        ret.minIndex = a.minIndex;
    else
        ret.minIndex = b.minIndex;
    return ret;
}

vector<Data> st; 
int stBase;

vector<vector<int>> g;
vector<int> startTime, endTime;


int discoveryTime;
void dfsSegtree (int node) {
    startTime[node] = discoveryTime++;
    for (int child : g[node])
        dfsSegtree(child);
    endTime[node] = discoveryTime;
};

void stSet(int i, Data value)
{
    i += stBase;
    st[i] = value;
    while (i > 0)
    {
        i /= 2;
        st[i] = op(st[2*i], st[2*i+1]);
    }
}

Data stGet(int i, int j)
{
    i += stBase;
    j += stBase;
    Data ret;
    while (i < j)
    {
        if (i % 2) ret = op(ret, st[i++]);
        if (j % 2) ret = op(ret, st[--j]);
        i /= 2;
        j /= 2;
    }
    return ret;
}

vector<long long> capByLeafCount;

bool alive(int node) { return !(st[stBase + startTime[node]] == NEUTRAL); }
bool leaf(int node) { return st[stBase + startTime[node]] == LEAF; }

void goGreedy(int node, long long prevSum)
{
    while (true)
    {
        assert(alive(node));
        if (leaf(node))
        {
            stSet(startTime[node], NEUTRAL);
            return;
        }
        Data subtree = stGet(startTime[node], endTime[node]);
        assert(subtree.minIndex >= 0);
        assert(subtree.leafCount > 0);
        long long delta = W[subtree.minIndex] - prevSum;
        assert(delta >= 0);
        prevSum += delta;
        capByLeafCount[subtree.leafCount] += delta;
        for (int child : g[subtree.minIndex])
        if (alive(child))
            goGreedy(child, prevSum);
        stSet(startTime[subtree.minIndex], LEAF);
    }
}

void init(vector<int> P, vector<int> localW) {
    W = localW;
    N = SIZE(P);
    for(stBase = 1; stBase < N; stBase *= 2);
    st = vector<Data>(2*stBase);
    g = vector<vector<int>>(N);
    forsn(i,1,N)
        g[P[i]].push_back(i);
    discoveryTime = 0;
    startTime = vector<int>(N);
    endTime = vector<int>(N);
    dfsSegtree(0);

    vector<bool> leaf(N, true);
    leafWeights = 0;
    dforn(i,N)
    {
        if (leaf[i])
        {
            leafWeights += W[i];
            st[stBase + startTime[i]] = LEAF;
        }
        else
            st[stBase + startTime[i]].minIndex = i;
        if (i > 0)
            leaf[P[i]] = false;
    }
    dforn(i,stBase)
        st[i] = op(st[2*i], st[2*i+1]);
    
    capByLeafCount = vector<long long>(N+1, 0);
    goGreedy(0, 0);
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
