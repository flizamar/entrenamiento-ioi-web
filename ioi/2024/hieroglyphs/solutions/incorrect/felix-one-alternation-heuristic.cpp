#include "hieroglyphs.h"
#include<bits/stdc++.h>

using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;

//erases non-common elements
void clean(vi& a, vi& b) {
    vi ap;
    vi bp;
    set<int> as;
    set<int> bs;
    for (int x : a) as.insert(x);
    for (int x : b) bs.insert(x);
    for (int x : a) if (bs.count(x)) ap.push_back(x);
    for (int x : b) if (as.count(x)) bp.push_back(x);
    swap(a, ap);
    swap(b, bp);
}

map<int, int> coordinate_compress(vi& a, vi& b) {
    int cc = 0;
    map<int, int> mp;
    map<int, int> rmp;
    for (int& x : a) {
        if (!mp.count(x)) {
            mp[x] = cc++;
            rmp[mp[x]] = x;
        }
        x = mp[x];
    }
    for (int& x : b) {
        if (!mp.count(x)) {
            mp[x] = cc++;
            rmp[mp[x]] = x;
        }
        x = mp[x];
    }
    return rmp;
}

bool is_subsequence(const vi& a, const vi& b) {
    int j = 0;
    for (int x : a) {
        if (j < (int)b.size() && b[j] == x) {
            j++;
        }
    }
    return j == (int)b.size();
}

vector<int> get_candidate(vector<int> a, vector<int> b) {
    int n = a.size();
    int m = b.size();

    vi occ_a(max(n, m)+1, 0);
    vi occ_b(max(n, m)+1, 0);
    for (int i=0; i < n; ++i) {
        occ_a[a[i]]++;
    }
    for (int i=0; i < m; ++i) {
        occ_b[b[i]]++;
    }

    vi c;
    queue<int> qa;
    queue<int> qb;

    for (int i=0; i < n; ++i) {
        if (occ_a[a[i]] <= occ_b[a[i]]) {
            qa.push(i);
        }
    }
    for (int i=0; i < m; ++i) {
        if (occ_a[b[i]] > occ_b[b[i]]) {
            qb.push(i);
        }
    }

    int i_a_curr = 0;
    int i_b_curr = 0;
    int i_a_next = 0;
    int i_b_next = 0;
    vi occ_a_curr = vi(occ_a);
    vi occ_a_next = vi(occ_a);
    vi occ_b_curr = vi(occ_b);
    vi occ_b_next = vi(occ_b);

    while(!qa.empty() && !qb.empty()) {
        while(i_a_next < qa.front()) {
            occ_a_next[a[i_a_next]]--;
            i_a_next++;
        }
        while(i_b_next < qb.front()) {
            occ_b_next[b[i_b_next]]--;
            i_b_next++;
        }

        int x = a[i_a_next];
        int y = b[i_b_next];

        int occ_x = occ_a_next[x];
        int occ_y = occ_b_next[y];

        bool a_good = (occ_a_next[y] >= occ_y && occ_b_curr[x] > occ_b_next[x]);
        bool b_good = (occ_b_next[x] >= occ_x && occ_a_curr[y] > occ_a_next[y]);

        if (a_good && b_good) return vi();
        if (!a_good && !b_good) return vi();

        if(a_good) {
            c.push_back(x);
            qa.pop();
            while(i_a_curr <= i_a_next) {
                occ_a_curr[a[i_a_curr]]--;
                i_a_curr++;
            }
            while(b[i_b_curr] != x) {
                occ_b_curr[b[i_b_curr]]--;
                i_b_curr++;
            }
            occ_b_curr[b[i_b_curr]]--;
            i_b_curr++;
        }
        else {
            c.push_back(y);
            qb.pop();
            while(i_b_curr <= i_b_next) {
                occ_b_curr[b[i_b_curr]]--;
                i_b_curr++;
            }
            while(a[i_a_curr] != y) {
                occ_a_curr[a[i_a_curr]]--;
                i_a_curr++;
            }
            occ_a_curr[a[i_a_curr]]--;
            i_a_curr++;
        }
    }

    while(!qa.empty()) {
        c.push_back(a[qa.front()]);
        qa.pop();
    }
    while(!qb.empty()) {
        c.push_back(b[qb.front()]);
        qb.pop();
    }

    return ((is_subsequence(a, c) && is_subsequence(b, c)) ? c : vi());
}



bool verify_greedy(vector<int> a, vector<int> b, vector<int> c) {
    int n = a.size();
    int m = b.size();
    int l = c.size();

    vi occ_a(max(n, m)+1, 0);
    vi occ_b(max(n, m)+1, 0);
    for (int i=0; i < n; ++i) {
        occ_a[a[i]]++;
    }
    for (int i=0; i < m; ++i) {
        occ_b[b[i]]++;
    }

    vi cv(l, n);
    int bi = 0;
    int ci = 0;

    for (int i=0; i < n; ++i) {
        if (occ_a[a[i]] >= occ_b[a[i]]) {
            while(bi < m && b[bi] != a[i]) bi++;
            if (bi == m) break;
            bi++;
            while (ci < l && c[ci] != a[i]) ci++;
            cv[ci] = i;
            ci++;
        }
    }

    int aj = n-1;
    int mina = n;
    int cj = l-1;
    for (int j=m-1; j > -1; --j) {
        if (occ_b[b[j]] >= occ_a[b[j]]) {
            while (cj > -1 && c[cj] != b[j]) {
                mina = min(mina, cv[cj]);
                cj--;
            }
            if (cj < 0) break;
            cj--;

            while (aj > -1 && a[aj] != b[j]) {
                aj--;
            }
            if (aj < 0) break;
            if (aj > mina) return false;
            aj--;
        }
        else {
            while (cj > -1 && c[cj] != b[j]) cj--;
            if (cj < 0) break;
            cj--;
        }
    }
    return true;
}

bool verify(vector<int> a, vector<int> b, vector<int> c) {
    if (c.empty()) return false;
    return verify_greedy(a, b, c) && verify_greedy(b, a, c);
}


vector<int> ucs(vector<int> a, vector<int> b) {
    clean(a, b);
    if (a.empty() || b.empty()) {
		return vector<int>();
    }
    map<int, int> mp = coordinate_compress(a, b);
    vi c = get_candidate(a, b);
    if (verify(a, b, c)) {
        for (int& x : c) x = mp[x];

		return c;
    }
    return {-1};
}
