#include <cstdio>
#include <set>
#include <algorithm>
#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <map>
using namespace std;

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

typedef long long llong;
const llong linf = 1e13;
const int inf = 1e9;

llong sqr(int x) {
    return 1ll * x * x;
}

chrono::high_resolution_clock::time_point start;
double wallTime() {
    auto cur = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(cur - start).count() / 1e9;
}

const int segs = 1;
const double timeLimit = 1.9;
mt19937 rng;

inline int rnd(int a, int b) {
    assert(a <= b);
    return uniform_int_distribution<int>(a, b)(rng);
}

vector<int> L, R;

struct state {
    set<int> in;
    vector<int> vout;
    vector<int> vin;
    llong score = 0;
    state(set<int> in) {
        this->in = in;
        vin = vector<int>(in.begin(), in.end());
        int prv = -1;
        int n = (int)L.size();
        score = val(-1, n - 1);
        for (int i = 0; i < n; i++) {
            if (!in.count(i))
                vout.push_back(i);
            else
                score += val(prv, i) + val(i, n - 1) - val(prv, n - 1), prv = i;
        }
    }
    llong val(int a, int b) {
        return sqr(1 + R[b] - L[a + 1]) - ((a != -1 && R[a] - L[a + 1] >= 0) ? sqr(R[a] - L[a + 1] + 1) : 0);
    }
    void mutate(double temp, double norm) {
        int ri = rnd(0, (int)vin.size() - 1);
        int ro = rnd(0, (int)vout.size() - 1);
        int y = vin[ri];
        auto it = in.find(y);
        assert(it != in.end());
        int x = -1, z = (int)R.size() - 1;
        if (it != in.begin()) {
            x = *(--it);
            ++it;
        }
        if (++it != in.end()) {
            z = *it;
        }
        --it;
        llong old_score = score;
        llong delta = 0;
        delta += -val(x, y) - val(y, z) + val(x, z);
        in.erase(it);
        swap(vin[ri], vout[ro]);
        x = -1;
        y = vin[ri];
        z = (int)R.size() - 1;
        auto res = in.insert(y);
        assert(res.second);
        it = res.first;
        if (it != in.begin()) {
            x = *(--it);
            ++it;
        }
        if (++it != in.end()) {
            z = *it;
        }
        --it;
        delta += val(x, y) + val(y, z) - val(x, z);
        llong new_score = old_score + delta;
        static int iter = 0;
        double prob = 1 - delta/(temp * norm * 20);
        if (delta >= 0) {
            if (++iter % 10000 == 0) {
                eprintf("iter = %d, prob = %.5lf\n", iter, prob);
            }
        }
        if (delta < 0 || bernoulli_distribution(prob)(rng)) {
            score = new_score;
        } else {
            in.erase(it);
            swap(vin[ri], vout[ro]);
            in.insert(vin[ri]);
        }
    }
};

long long take_photos(int n, int m, int k, std::vector<int> row, std::vector<int> column) {
    map<int, int> M;
    for (int i = 0; i < n; i++) {
        int x, y;
        x = row[i];
        y = column[i];
        if (x < y)
            swap(x, y);
        if (!M.count(x))
            M[x] = x;
        M[x] = min(M[x], y);
    }
    for (auto pr : M) {
        while (!L.empty() && L.back() >= pr.second)
            L.pop_back(), R.pop_back();
        R.push_back(pr.first);
        L.push_back(pr.second);
    }
    n = L.size();
    k = min(k, n);
    assert(is_sorted(R.begin(), R.end()));
    assert(is_sorted(L.begin(), L.end()));
    vector<int> vec(k - 1);
    for (int i = 0; i < k - 1; i++) {
        vec[i] = rnd(0, n - k);
    }
    llong ans = linf;
    for (int it = 0; it < segs; it++) {
        start = chrono::high_resolution_clock::now();
        double temp1 = 1e2;
        double temp2 = 1e-2;
        vector<int> tmp;
        for (int i = 0; i < n - 1; i++) {
            tmp.push_back(i);
        }
        shuffle(tmp.begin(), tmp.end(), rng);

        state cur(set<int>(tmp.begin(), tmp.begin() + k - 1));
        double init = cur.score;
        shuffle(tmp.begin(), tmp.end(), rng);
        state cur2(set<int>(tmp.begin(), tmp.begin() + k - 1));
        double init2 = cur.score;
        double norm = max(1.0, fabs(init - init2));
        ans = min(ans, (llong)init);
        if (k == 1 || k == n) {
            return ans;
        }
        int iter = 0;
        while (true) {
            double t = wallTime() / (timeLimit / segs);
            if (t > 1)
                break;
            double temp = exp(log(temp1) * (1 - t) + log(temp2) * t);
            cur.mutate(temp, norm);
            if (ans > cur.score) {
                ans = min(ans, cur.score);
                eprintf("iter = %d, ans = %lld\n", iter, ans);
            }
            if (iter % 100000 == 0) {
                eprintf("iter = %d\n", iter);
            }
            ++iter;
        }
    }
    return ans;
}

// BEGIN CUT

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    std::vector<int> r(n), c(n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &r[i], &c[i]);
    }
    long long ans = take_photos(n, m, k, r, c);

    // BEGIN SECRET
    puts("098d134608c94f7413faac591054ee35");
    // END SECRET

    printf("%lld\n", ans);
    return 0;
}

// END CUT
