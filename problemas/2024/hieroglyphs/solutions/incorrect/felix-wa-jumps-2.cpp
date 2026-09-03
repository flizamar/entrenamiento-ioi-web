#include<bits/stdc++.h>
#include "hieroglyphs.h"

using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;
using ii = pair<int, int>;

int ALPHABET_SIZE = 0;

bool is_subsequence(const vi& a, const vi& b) {
    int j = 0;
    for (int x : a) {
        if (j < (int)b.size() && b[j] == x) {
            j++;
        }
    }
    return j == (int)b.size();
}

vi get_candidate(const vi& a, const vi& b) {
    int n = a.size();
    int m = b.size();

    vi occ_a(ALPHABET_SIZE, 0);
    vi occ_b(ALPHABET_SIZE, 0);
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

        if (a_good && b_good) return {-1};
        if (!a_good && !b_good) return {-1};

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

    return ((is_subsequence(a, c) && is_subsequence(b, c)) ? c : vi({-1}));
}



bool verify_jump(const vi& a, const vi& b, const vi& c) {
    if (c == vi({-1})) return false;
    if (c == vi()) return true; 
    int n = a.size();
    int m = b.size();
    int l = c.size();

    vi occ_a(ALPHABET_SIZE, 0);
    vi occ_b(ALPHABET_SIZE, 0);
    vi occ_c(ALPHABET_SIZE, 0);
    vvi pos_a(ALPHABET_SIZE);
    vvi pos_b(ALPHABET_SIZE);
    vvi pos_c(ALPHABET_SIZE);
    for (int i=0; i < n; ++i) {
        occ_a[a[i]]++;
        pos_a[a[i]].push_back(i);
    }
    for (int i=0; i < m; ++i) {
        occ_b[b[i]]++;
        pos_b[b[i]].push_back(i);
    }
    for (int i=0; i < l; ++i) {
        occ_c[c[i]]++;
        pos_c[c[i]].push_back(i);
    }

    vi pos_c_idx(ALPHABET_SIZE);
    vi jump_left(l);
    for (int i=0; i < l; ++i) jump_left[i] = i;
    int c_idx = 0;

    for (int j=0; j < m; ++j) {
        
        if (b[j] == c[c_idx]) {
            pos_c_idx[b[j]]++;
            c_idx++;
            if (c_idx == l) break;
        }
        else {
            int idx = pos_c_idx[b[j]];
            if (idx < occ_c[b[j]]) {
                jump_left[pos_c[b[j]][idx]] = min(jump_left[pos_c[b[j]][idx]], c_idx);
                //pos_c_idx[b[j]]++;
            }
        }
        
    }

    vi jump_right(l);
    for (int i=0; i < l; ++i) jump_right[i] = i;
    c_idx--;
    
    for (int i=n-1; i > -1; --i) {
         
        if (a[i] == c[c_idx]) {
            pos_c_idx[a[i]]--;
            c_idx--;
            if (c_idx < 0) break;
        }
        else {
            int idx = pos_c_idx[a[i]]-1;
            if (idx > -1) {
                jump_right[pos_c[a[i]][idx]] = max(jump_right[pos_c[a[i]][idx]], c_idx);
                //pos_c_idx[a[i]]--;
            }
        }
        
    }

    vector<ii> stack_jump;


    for (int k=0; k < l; ++k) {
        while (!stack_jump.empty() && stack_jump.back().second < k) {
            stack_jump.pop_back();
        }
        if (!stack_jump.empty() && stack_jump.back().first >= jump_left[k]) {
            return false;
        }
        while (!stack_jump.empty() && stack_jump.back().second < jump_right[k]) {
            stack_jump.pop_back();
        }
        stack_jump.emplace_back(k, jump_right[k]);
        
    }

    
   

   
    return true;
}

bool verify(const vi& a, const vi& b, const vi& c) {
    return verify_jump(a, b, c) && verify_jump(b, a, c);
}

vector<int> ucs(vector<int> a, vector<int> b) {
    for (int x : a) ALPHABET_SIZE = max(ALPHABET_SIZE, x+1);
    for (int x : b) ALPHABET_SIZE = max(ALPHABET_SIZE, x+1);

    vi c = get_candidate(a, b);
    if (verify(a, b, c)) return c;
    return {-1};
}
