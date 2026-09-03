#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <cmath>
#include <cassert>
#include <ctime>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>

using namespace std;

#define pb push_back
#define mp make_pair
#define fs first
#define sc second

const int size = 100 * 1000;
const int magic = 100;

vector <int> vertex[size];
queue <int> bq[2];
int col[size];
int timecnt[2];
bool used[2][size];
int intime[size];

vector <int> try_random_bfs(int n, int a, int b, int c) {
    for (int i = 0; i < n; i++) {
        col[i] = -1;
        used[0][i] = used[1][i] = false;
        intime[i] = 0;
    }
    timecnt[0] = timecnt[1] = 0;

    int f = rand() % n;
    int s = (f + rand() % (n - 1)) % n;

    //bq[0].clear();
    //bq[1].clear();

    bq[0].push(f);
    bq[1].push(s);
    int swtch = 0;
    while (!bq[0].empty() || !bq[1].empty()) {
        int cur = -1;
        while (cur == -1 && !bq[swtch].empty()) {
            cur = bq[swtch].front();
            bq[swtch].pop();
            if (col[cur] != -1) {
                cur = -1;
            }
        }

        if (cur != -1) {
            col[cur] = swtch;
            intime[cur] = timecnt[swtch];
            timecnt[swtch]++;

            for (int e : vertex[cur]) {
                if (!used[swtch][e]) {
                    used[swtch][e] = true;
                    bq[swtch].push(e);
                }
            }
        }

        swtch ^= 1;
    }

    vector <pair <int, int> > sizes;
    sizes.pb(mp(a, 1));
    sizes.pb(mp(b, 2));
    sizes.pb(mp(c, 3));

    sort(sizes.begin(), sizes.end());

    int colcnt[2];
    colcnt[0] = colcnt[1] = 0;
    for (int i = 0; i < n; i++) {
        colcnt[col[i]]++;
    }

    vector <pair <int, int> > colsizes;
    colsizes.pb(mp(colcnt[0], 0));
    colsizes.pb(mp(colcnt[1], 1));
    sort(colsizes.begin(), colsizes.end());

    if (colsizes[0].fs < sizes[0].fs || colsizes[1].fs < sizes[1].fs) {
        return vector<int>();
    }

    vector <int> ans(n);
    for (int i = 0; i < n; i++) {
        int ord = -1;
        for (int j = 0; j < 2; j++) {
            if (colsizes[j].sc == col[i]) {
                ord = j;
            }
        }

        if (intime[i] < sizes[ord].fs) {
            ans[i] = sizes[ord].sc;
        } else {
            ans[i] = sizes[2].sc;
        }
    }

    return ans;
}

vector<int> find_split(int n, int a, int b, int c, vector <int> p, vector <int> q) {
    for (int i = 0; i < n; i++) {
        vertex[i].clear();
    }
    int m = p.size();
    for (int i = 0; i < m; i++) {
        vertex[p[i]].pb(q[i]);
        vertex[q[i]].pb(p[i]);
    }

    for (int it = 0; it < magic; it++) {
        vector <int> res = try_random_bfs(n, a, b, c);
        if (!res.empty()) {
		return res;
	}
    }

    return vector<int>(n, 0);
}
