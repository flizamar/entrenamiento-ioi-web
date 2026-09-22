// Problem: Tree
// Solution: very simple & slow, no leaves
// Time: n^2 * log(n) + q * n^2 * h
//  (h = tree height)
// Author: Kian Mirjalali

#include "tree.h"
#include <algorithm>
#include <iostream>
using namespace std;


#define allOf(c) (std::begin(c)), (std::end(c))
#define fori(i, a, b) for (int i = (a); (i < (b)); i++)
#define forn(i, n) fori(i, 0, n)
#define forv(i, v) forn(i, sz(v))
#define forRev(i, b, a) for (int i = ((b)-1); (i >= (a)); i--)

template<class C>
int sz(const C& c) {
    return c.size();
}

template<class A, class B>
bool minEq(A& a, const B& b) {
    return (b < a) && ((a = b), true);
}


template<class C1, class C2>
void append(C1& c1, const C2& c2) {
    c1.insert(std::end(c1), allOf(c2));
}


using Long = long long;
using VI = vector<int>;

int n;
VI p, w;

vector<VI> children;
vector<VI> subWSort;

bool wComp(int i, int j) {
    return w[i] < w[j];
}

void init(VI P, VI W) {
    p = P;
    w = W;
    n = p.size();
    children.assign(n, VI());
    fori(i, 1, n)
        children[p[i]].push_back(i);
    subWSort.assign(n, VI());
    // cerr << "B_init" << endl;
    forRev(u, n, 0) {
        if (children[u].empty())
            continue;
        auto& ws = subWSort[u];
        ws.push_back(u);
        for (int child : children[u])
            append(ws, subWSort[child]);
    }
    // cerr << "M_init" << endl;
    for (auto& ws : subWSort)
        sort(allOf(ws), wComp);
    // cerr << "EO_init" << endl;
}

Long query(int L, int R) {
    if (L*(long long)R <= 0) return 0;
    if (L < 0) return query(-R, -L);
    vector<Long> coef(n), subCSum(n);
    forRev(u, n, 0) {
        if (children[u].empty()) {
            subCSum[u] = coef[u] = L;
            continue;
        }
        subCSum[u] = coef[u] = 0;
        for (int child : children[u])
            subCSum[u] += subCSum[child];
        for (int v : subWSort[u]) {
            if (subCSum[u] <= R)
                break;
            Long d = subCSum[u]-R;
            // d > 0
            for (int x = v; x != p[u]; x = p[x])
                minEq(d, subCSum[x]-L);
            coef[v] -= d;
            for (int x = v; x != p[u]; x = p[x])
                subCSum[x] -= d;
        }
    }

    Long cost = 0;
    forn(i, n)
        cost += abs(coef[i])*w[i];
    return cost;
}
