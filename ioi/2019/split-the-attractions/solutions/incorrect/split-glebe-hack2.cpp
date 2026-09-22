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
const int magic_max = 20;
//const int ops = 100 * 1000 * 1000;
const int iters = 100;
const double timelimit = 1.5;

vector <int> vertex[size];
queue <int> bq[magic_max];
int col[size];
bool used[magic_max][size];
int intime[size];
bool used_small[magic_max];
bool connected[1 << magic_max];
unsigned int way[magic_max];
vector <pair <int, int> > edges;
vector <int> vertex_tree[size];
int par[size];
int depth[size];
int subsize[size];

void tree_dfs(int v, int from) {
    subsize[v] = 1;
    for (int e : vertex_tree[v]) {
        if (e != from) {
	    depth[e] = depth[v] + 1;
	    tree_dfs(e, v);
	    subsize[v] += subsize[e];
	}
    }
}

int getpar(int v) {
    if (par[v] != v) {
        par[v] = getpar(par[v]);
    }

    return par[v];
}

void build_random_span(int n) {
    for (int i = 0; i < n; i++) {
    	vertex_tree[i].clear();
	par[i] = i;
    }

    random_shuffle(edges.begin(), edges.end());
    int m = edges.size();

    for (int i = 0; i < m; i++) {
    	int u = edges[i].fs;
	int v = edges[i].sc;

	u = getpar(u);
	v = getpar(v);
	if (u != v) {
	    vertex_tree[edges[i].fs].pb(edges[i].sc);
            vertex_tree[edges[i].sc].pb(edges[i].fs);
	}

	par[u] = v;
    }
}

int find_centroid(int n) {
    depth[0] = 0;
    tree_dfs(0, -1);

    int cen = 0;
    for (int i = 0; i < n; i++) {
        if (subsize[i] >= (n + 1) / 2 && depth[i] > depth[cen]) {
	    cen = i;
	}
    }

    return cen;
}
/*
vector <int> random_span_sol(int n, int a, int b, int c) {
    build_random_span(n);
    random_shuffle(edges.begin(), edges.end());

    int cen = find_centroid(n);
}
*/
int random_span_centroid(int n) {
    build_random_span(n);
    random_shuffle(edges.begin(), edges.end());

    return find_centroid(n);
}

void recolor(int n, int tg, int res, int cnt, unsigned int mask, vector <int>& ans) {
    queue <int> q;
    int stp = -1;
    for (int i = 0; i < n; i++) {
        used[0][i] = false;
    	if ((mask >> col[i]) & 1) {
            stp = i;
	}
    }

    q.push(stp);
    used[0][stp] = true;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (cnt > 0) {
            ans[v] = tg;
            cnt--;
        } else {
            ans[v] = res;
        }

        for (int e : vertex[v]) {
            if (!used[0][e] && ((mask >> col[e]) & 1)) {
                q.push(e);
                used[0][e] = true;
            }
        }
    }
}

void run_parallel_bfs(int n, const vector<int>& nodes) {
    for (int i = 0; i < n; i++) {
        col[i] = -1;
    }

    int magic = nodes.size();

    for (int i = 0; i < magic; i++) {
        for (int j = 0; j < n; j++) {
            used[i][j] = false;
        }
    }

    for (int i = 0; i < magic; i++) {
    	bq[i].push(nodes[i]);
        used[i][nodes[i]] = true;
    }

    int used_cnt = 0;
    bool flag = true;
    while (flag) {
        flag = false;
        for (int i = 0; i < magic; i++) {
            int cur = -1;
            while (!bq[i].empty() && cur == -1) {
                cur = bq[i].front();
                bq[i].pop();
                if (col[cur] != -1) {
                    cur = -1;
                }
            }
            if (cur == -1) {
                continue;
            }
	    used_cnt++;
            flag = true;

            col[cur] = i;
	    if (i == magic - 1) {
	    	continue;
	    }
            for (int e : vertex[cur]) {
                if (!used[i][e] && col[e] == -1) {
                    used[i][e] = true;
                    bq[i].push(e);
                }
            }
        }
    }

    if (used_cnt < n) {
    	bq[magic - 1].push(nodes[magic - 1]);
	while (!bq[magic - 1].empty()) {
	    int cur = bq[magic - 1].front();
	    bq[magic - 1].pop();

	    col[cur] = magic - 1;
	    for (int e : vertex[cur]) {
                if (col[e] == -1) {
		    col[e] = magic - 1;
		    bq[magic - 1].push(e);
		}
	    }
	}
    }
}

void build_colors_graph(int n, int magic) {
    for (int i = 0; i < magic; i++) {
        for (int j = 0; j < magic; j++) {
            way[i] = 0u;
        }
    }
    for (int i = 0; i < magic; i++) {
        way[i] |= (1u << i);
    }

    for (int i = 0; i < n; i++) {
        for (int e : vertex[i]) {
            way[col[i]] |= (1u << col[e]);
            way[col[e]] |= (1u << col[i]);
        }
    }

    connected[0] = true;
    for (unsigned int mask = 1u; mask < (1u << magic); mask++) {
	connected[mask] = false;
    	for (int i = 0; i < magic; i++) {
	    if ((mask >> i) & 1) {
		unsigned int newmask = mask ^ (1 << i);
	        if (newmask == 0u) {
		    connected[mask] = true;
		    break;
		}
		if (connected[newmask] && (newmask & way[i])) {
	            connected[mask] = true;
		    break;
		}
	    }
	}
    }
}

void dfs_small(int magic, int v, unsigned int mask) {
    used_small[v] = true;
    for (int i = 0; i < magic; i++) {
        if (((way[v] >> i) & 1) && ((mask >> i) & 1) && !used_small[i]) {
            dfs_small(magic, i, mask);
        }
    }
}

bool is_connected(int magic, unsigned int mask) {
    //cerr << magic << ' ' << mask << endl;
    for (int i = 0; i < magic; i++) {
        used_small[i] = false;
    }
    int sp = -1;
    for (int i = 0; i < magic; i++) {
        if ((mask >> i) & 1) {
            sp = i;
            break;
        }
    }

    if (sp == -1) {
        return true;
    }

    dfs_small(magic, sp, mask);

    for (int i = 0; i < magic; i++) {
        if (((mask >> i) & 1) && !used_small[i]) {
            return false;
        }
    }

    return true;
}

vector <int> rand_order;

vector <int> try_random_bfs(int n, int a, int b, int c, int magic) {
    for (int i = 0; i < n; i++) {
        random_shuffle(vertex[i].begin(), vertex[i].end());
    }
    random_shuffle(rand_order.begin(), rand_order.end());

    vector <int> nodes = {rand_order.begin(), rand_order.begin() + magic};

    int cen = random_span_centroid(n);
    for (int i = 0; i < (int) nodes.size(); i++) {
        if (nodes[i] == cen) {
	    swap(nodes[i], nodes.back());
	}
    }
    nodes[(int) nodes.size() - 1] = cen;

    run_parallel_bfs(n, nodes);

    vector <pair <int, int> > sizes;
    sizes.pb(mp(a, 1));
    sizes.pb(mp(b, 2));
    sizes.pb(mp(c, 3));

    sort(sizes.begin(), sizes.end());

    vector <int> colcnt(magic, 0);
    for (int i = 0; i < n; i++) {
        colcnt[col[i]]++;
    }

    build_colors_graph(n, magic);

    unsigned int allmask = (1 << magic) - 1;
    for (unsigned int mask = 0u; mask <= allmask; mask++) {
        unsigned int sup = allmask ^ mask;

        int cnt0 = 0;
        int cnt1 = 0;
        for (int i = 0; i < magic; i++) {
            if ((mask >> i) & 1) {
                cnt0 += colcnt[i];
            }
            if ((sup >> i) & 1) {
                cnt1 += colcnt[i];
            }
        }

        if (cnt0 > cnt1) {
            continue;
        }
        if (sizes[0].fs > cnt0 || sizes[1].fs > cnt1) {
            continue;
        }

        if (connected[mask] && connected[sup]) {
            vector <int> ans(n, -1);
            recolor(n, sizes[0].sc, sizes[2].sc, sizes[0].fs, mask, ans);
            recolor(n, sizes[1].sc, sizes[2].sc, sizes[1].fs, sup, ans);
/*
	    for (int i = 0; i < n; i++) {
	        cerr << col[i] << ' ';
	    }
	    cerr << endl;
            for (int i = 0; i < magic; i++) {
                cerr << ans[i] << ' ';
	    }
	    cerr << endl;
*/
            return ans;
        }
    }

    return vector <int>();
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
    for (int i = 0; i < m; i++) {
    	edges.pb(mp(p[i], q[i]));
    }

    for (int i = 0; i < n; i++) {
        rand_order.pb(i);
    }

    //for (int it = 0; it < iters; it++) {
    while ((clock() + 0.0) / CLOCKS_PER_SEC < timelimit) {
        vector <int> res = try_random_bfs(n, a, b, c, min(magic_max, n));
        if (!res.empty()) {
            return res;
	}
    }

    return vector<int>(n, 0);
}
