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

vector<int> ucs_quad(vi a, vi b) {
	vi c = lcs(a, b);

	int n = a.size();
	int m = b.size();
	int l = c.size();

	vvi nxt(max(n, m)+1, vi(l+2, l)); //nxt[x][i] = first k such that c[k] = x and k >= i, l if such k does not exist

	for (int i=0; i < l; ++i) {
		for (int j=0; j <= i; ++j) {
			nxt[c[i]][j] = min(nxt[c[i]][j], i);
		}
	}

	vvi dp(n+1, vi(m+1, -1)); //dp[i][j] = maximum k so that there is a common subseq of a[0..i), b[0..j) which is not a subseq of c[0..k), -1 if no subseq

	for (int i=1; i <= n; ++i) {
		for (int j=1; j <= m; ++j) {
			if (a[i-1] == b[j-1]) {
				dp[i][j] = nxt[a[i-1]][dp[i-1][j-1]+1];
			}
			dp[i][j] = max(dp[i][j], max(dp[i-1][j], dp[i][j-1]));
		}
	}

	if(dp[n][m] == l) return vector<int>({-1});
	return c;
}

vector<int> ucs(vector<int> a, vector<int> b) {
    clean(a, b);
    if (a.empty() || b.empty()) {
		return vector<int>();
    }
    map<int, int> mp = coordinate_compress(a, b);
    vi c;
    if ((int)a.size() <= 500 && (int)b.size() <= 500) c = ucs_quad(a, b);
    else {
        c = get_candidate(a, b);
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        vi rc = get_candidate(a, b);
        reverse(rc.begin(), rc.end());
        if (c != rc) return {-1};
    }
    if (c.empty() || c[0] == -1) return {-1};
    for (int& x : c) x = mp[x];
    return c;
}
