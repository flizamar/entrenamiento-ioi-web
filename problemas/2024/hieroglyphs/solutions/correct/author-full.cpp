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




//returns v of size |b|
//v[i] = smallest index so that there exists a common subsequence of a[0..v[i]] and b[0..i] that is not
// a common subsequence of a[0..v[i]] and b[0..i-1]. if such index does not exist, v[i] = |a|
vector<int> index_vector(const vi& a, const vi& b) {
    int n = a.size();
    int m = b.size();
    vi v(m);
    int max_v = 0;
    for (int x : a) max_v = max(x, max_v);
    for (int x : b) max_v = max(x, max_v);
    vi prev_occ_b(max_v+1, -1);
    vvi a_occ(max_v+1);
    for (int i=0; i < n; ++i) {
        a_occ[a[i]].push_back(i);
    }
    for (int i=0; i <= max_v; ++i) {
        a_occ[i].push_back(n);
    }
    vector<pair<int, int>> min_stack;
    for (int i=0; i < m; ++i) {
        if (prev_occ_b[b[i]] == -1) {
            v[i] = a_occ[b[i]][0];
        }
        else {
            int min_val = lower_bound(min_stack.begin(), min_stack.end(), pair<int, int>(prev_occ_b[b[i]], -1))->second;
            if (min_val < n) v[i] = *lower_bound(a_occ[b[i]].begin(), a_occ[b[i]].end(), min_val+1);
            else v[i] = n;
        }
        while(!min_stack.empty() && min_stack.back().second >= v[i]) {
            min_stack.pop_back();
        }
        min_stack.emplace_back(i, v[i]);
        prev_occ_b[b[i]] = i;
    }
    return v; 
}

// RMQ template from KACTL
template<class T>
struct RMQ {
	vector<vector<T>> jmp;
	RMQ(const vector<T>& V) : jmp(1, V) {
		for (int pw = 1, k = 1; pw * 2 <= (int)V.size(); pw *= 2, ++k) {
			jmp.emplace_back((int)V.size() - pw * 2 + 1);
			for (int j=0; j < (int)jmp[k].size(); ++j)
				jmp[k][j] = min(jmp[k - 1][j], jmp[k - 1][j + pw]);
		}
	}
	T query(int a, int b) {
		int dep = 31 - __builtin_clz(b - a);
		return min(jmp[dep][a], jmp[dep][b - (1 << dep)]);
	}
};

bool no_single_crossing(const vi& a, const vi& b, const vi& c) {
    int n = a.size();
    int m = b.size();
    int l = c.size(); 

    vi rb = vi(b);
    reverse(rb.begin(), rb.end());

    vi rc = vi(c);
    reverse(rc.begin(), rc.end());

    vi v_ab = index_vector(a, b);
    vi v_rbc = index_vector(rb, rc);
    
    RMQ<int> v_rbc_rmq = RMQ<int>(v_rbc);

    vi v_max_rc_i(n+1);
    v_max_rc_i[n] = 0;
    for (int i=n-1; i >= 0; --i) {
        v_max_rc_i[i] = v_max_rc_i[i+1];
        if (a[i] == rc[v_max_rc_i[i+1]]) {
            v_max_rc_i[i]++;
        }
        if (v_max_rc_i[i] > l) v_max_rc_i[i] = l;
    }
    vi v_min_rc_i(m+1);
    v_min_rc_i[0] = l-1;
    for (int i=0; i < m; ++i) {
        v_min_rc_i[i+1] = v_min_rc_i[i];
        if (b[i] == rc[v_min_rc_i[i]]) {
            v_min_rc_i[i+1]--;
        }
        if (v_min_rc_i[i] < -1) v_min_rc_i[i] = -1;
    }

    for (int j=0; j < m; ++j) {
        int ai = v_ab[j];
        if (ai == n) continue;
        int min_rc_i = v_min_rc_i[j+1];
        int max_rc_i = v_max_rc_i[ai+1];
        if (min_rc_i+1 < max_rc_i && v_rbc_rmq.query(min_rc_i+1, max_rc_i) + j < m-1) return false;
    }
    return true;
}

bool verify(const vi& a, const vi& b, const vi& c) {
    if (c.empty()) return false;
    return no_single_crossing(a, b, c) && no_single_crossing(b, a, c);    
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
    return vector<int>({-1});
}
