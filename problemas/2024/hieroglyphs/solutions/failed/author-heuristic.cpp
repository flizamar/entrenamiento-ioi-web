#include<bits/stdc++.h>
#include "hieroglyphs.h"

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

bool compressed(const vi& a, const vi& b) {
    set<int> as;
    set<int> bs;
    int n = a.size();
    int m = b.size();
    for (int x : a) as.insert(x);
    for (int x : b) bs.insert(x);
    for (int x : a) {
        if (x >= n) return false;
        if (!bs.count(x)) return false;
    }
    for (int x : b) {
        if (x >= m) return false;
        if (!as.count(x)) return false;
    }
    return true;
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

vector<int> get_candidate_linear(vector<int> a, vector<int> b) {
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

vi reverse_get_candidate_linear(vi a, vi b) {
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    vi c = get_candidate_linear(a, b);
    reverse(c.begin(), c.end());
    return c;
}

bool verify_xxyy_subseq(vector<int> a, vector<int> b, vector<int> c) {
    if (c.empty()) return false;
    int n = a.size();
    int m = b.size(); 
    int l = c.size();
    set<int> chars;
    for (int x : c) {
        chars.insert(x);
    }
    for (int x : chars) {
        for (int y : chars) {
            if (x == y) continue;
            int yac = 0;
            int ybc = 0;
            int ycc = 0;
            for (int i=0; i < n; ++i) if (a[i] == y) yac++; 
            for (int i=0; i < m; ++i) if (b[i] == y) ybc++; 
            for (int i=0; i < l; ++i) if (c[i] == y) ycc++; 
            int i = 0;
            int j = 0;
            int k = 0; 
            while (i < n && j < m && k < l) {
                while (i < n && a[i] != x) {
                    if (a[i] == y) yac--;
                    i++;
                }
                while (j < m && b[j] != x) {
                    if (b[j] == y) ybc--;
                    j++;
                }
                while (k < l && c[k] != x) {
                    if (c[k] == y) ycc--;
                    k++;
                }
                if (i < n && j < m && k < l) {
                    if (yac > ycc && ybc > ycc) {
                        return false;
                    }
                    i++;
                    j++;
                    k++;
                }
            }
        }
    }
    return true;
}

//complexity not optimized
pair<bool, vi> solve(vi a, vi b) {
    if (a.empty() || b.empty()) {
        return pair<bool, vi>(true, {});
    }
    if (a.back() == b.back()) {
        int x = a.back();
        a.pop_back();
        b.pop_back();
        auto p = solve(a, b);
        if (p.first) {
            p.second.push_back(x);
        }
        return p;
    }
    if (a[0] == b[0]) {
        int x = a[0];
        a.erase(a.begin());
        b.erase(b.begin());
        auto p = solve(a, b);
        if (p.first) {
            p.second.insert(p.second.begin(), x);
        }
        return p;
    }
    if (!compressed(a, b)) {
        clean(a, b);
        if (a.empty() || b.empty()) {
            return pair<bool, vi>(true, {});
        }
        map<int, int> mp = coordinate_compress(a, b);
        auto p = solve(a, b);
        for (int& x : p.second) x = mp[x];
        return p;
    }

    //End recursive solving part

    vector<function<vi(vi, vi)>> candidates_f = {get_candidate_linear, reverse_get_candidate_linear, lcs};

    vi c = candidates_f[0](a, b);
    
    for (auto f : candidates_f) {
        if (c != f(a, b)) return pair<bool, vi>(false, {});
    }

    vector<function<bool(vi, vi, vi)>> verify_f = {verify_xxyy_subseq};

    for (auto f : verify_f) {
        if (!f(a, b, c)) return pair<bool, vi>(false, {});
    }

    return pair<bool, vi>(true, c);
}

vector<int> ucs(vector<int> a, vector<int> b) {
    auto p = solve(a, b);
    if (p.first)
        return p.second;
    return vector<int> (1, -1);
}

