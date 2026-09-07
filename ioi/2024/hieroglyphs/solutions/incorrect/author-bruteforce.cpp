#include<bits/stdc++.h>
#include "hieroglyphs.h"

using namespace std;

using vi = vector<int>;

bool is_subsequence(const vi& a, const vi& b) {
    int j = 0;
    for (int x : a) {
        if (j < (int)b.size() && b[j] == x) {
            j++;
        }
    }
    return j == (int)b.size();
}

vector<int> ucs(vector<int> a, vector<int> b) {
    if (a.size() < b.size()) swap(a, b);
    //int n = a.size();
    int m = b.size();
    vector<vi> subs;
    vi max_sub;
    for (int mask=0; mask < (1<<m); ++mask) {
        vi cs;
        for (int j=0; j < m; ++j) {
            if (mask&(1<<j)) cs.push_back(b[j]);
        }
        if (is_subsequence(a, cs))  {
            subs.push_back(cs);
            if (cs.size() > max_sub.size()) max_sub = cs;
        }
    }
    for (vi s : subs) {
        if (!is_subsequence(max_sub, s)) {
            return vector<int>({-1});
        }
    }
    return max_sub;
}

