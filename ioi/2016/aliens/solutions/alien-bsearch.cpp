#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

#define ll long long
#define MAXN 1000001
#define pb push_back
#define INFI (~(1LL<<63))
#define sq(x) ((x) * (x))
#define DELTA 0.000001

struct range{
 ll s, e;
 friend bool operator < (const range &a, const range &b) {
  if (a.s == b.s)
   return (a.e > b.e) ? true : false;
  else
   return (a.s < b.s) ? true : false;
  }
};


struct point {
 ll x, y;
};

struct line {
 ll m, c;
};

struct hull_line {
 ll m, c;
 ll k;
};

class Hull {
private:
 int cur, len;
 hull_line *L;
 double getx(ll m1, ll c1, ll m2, ll c2);

public:
 void addline(ll m, ll c, ll k);
 pair<ll,ll> getmin(ll x);
 void resetline();
 Hull();
 ~Hull();
};

Hull::Hull() {
 L = (hull_line*)malloc(MAXN * sizeof(hull_line));
}

Hull::~Hull() {
 free(L);
}

double Hull::getx(ll m1, ll c1, ll m2, ll c2) {
 return (c2-c1) / (double)(m1-m2);
}

void Hull::addline(ll m, ll c, ll k) {
 while (len >= 2) {
  double a = getx(m, c, L[len-1].m, L[len-1].c);
  double b = getx(m, c, L[len-2].m, L[len-2].c);
  if (a > b) break;
  len--;
 }
 L[len].m = m;
 L[len].c = c;
 L[len].k = k;
 len++;
}

pair<ll,ll> Hull::getmin(ll x) {
 if (cur >= len) cur = len-1;
 while (cur < len-1) {
  double a = getx(L[cur].m, L[cur].c, L[cur+1].m, L[cur+1].c);
  if (x > a)
   cur++;
  else
   break;
   // in case x==a first such line is considered
   // It can be proven using convex property
   // that k increase as cur increases
 }
 return make_pair(L[cur].m * x + L[cur].c, L[cur].k);
}

void Hull::resetline() {
 len = 0;
 cur = 0;
}


pair<ll,ll> hull(vector<range> &r, ll C, Hull &h) {
 int n = r.size();
 ll m, c;
 ll t, lastRes = 0LL, lastK = 0LL;
 h.resetline();
 pair<ll,ll> res;
 for (int i=0; i<n; ++i) {
  m = -2LL * (r[i].s - 1LL);
  c = lastRes + sq(r[i].s - 1LL);
  if (i > 0) {
   t = max(0LL, r[i-1].e - r[i].s + 1LL);
   c -= sq(t);
  }
  h.addline(m, c, lastK);
  res = h.getmin(r[i].e);
  lastRes = sq(r[i].e) + C + res.first;
  lastK = res.second + 1LL;
 }
 return make_pair(lastRes, lastK);;
}

ll find(vector<range> &r, int K, int M) {
 Hull h;
 int N = r.size();
 K = min(K, N);
 ll kl=1LL, kr=(ll)N, cl = sq((ll)M), cr=0LL;
 ll med;
 bool found = false;
 pair<ll,ll> res;
 if (cl % 2LL == 1LL) cl++; // keep C left to an even integer
 // run binary search on C=[cl,cr]
 while (kr-kl > 1LL && cl - cr > 1LL) {
  med = cr + (cl-cr)/2LL;
  // if (med % 2LL == 1LL) med--; // keep med even
  res = hull(r, med + 1LL, h);
  if (K == res.second) {
   found = true;
   break;
  }
  if (K < res.second) {
   cr = med;
   kr = res.second;
  } else {
   cl = med;
   kl = res.second;
  }
 }
 if (found) {
  return res.first - (med+1LL) * K;
 } else if (kr - kl > 1) {
  // slope is equal from kl to kr.
  // Since odd C values are used in hull() it always returns
  // leftmost k out of those slopes for given C. i.e. slopes are always even
  res = hull(r, cl+1LL, h);
  ll fl = res.first - (cl + 1LL) * kl;
  res = hull(r, cr+1LL, h);
  ll fr = res.first - (cr + 1LL) * kr;
  ll h = (fl-fr) / (kr - kl); // interpolate
  return fl - (K - kl) * h;
 } else {
  if (kr == K) {
   res = hull(r, cr + 1LL, h);
   return res.first - (cr + 1LL) * K;
  } else { //kr = K
   res = hull(r, cl + 1LL, h);
   return res.first - (cl + 1LL) * kl;
  }
 }
}

vector<range> parseRanges(range* r, int n) {
 sort(&r[0], &r[n]);
 vector<range> res;
 int cur = 0;
 res.pb(r[0]);
 for (int i=1; i<n; ++i)
  if (r[cur].e < r[i].s || r[cur].e < r[i].e) {
   cur = i;
   res.pb(r[i]);
  }
 return res;
}

long long take_photos(int n, int m, int k, vector<int> row, vector<int> col) {
 point *p = (point*)malloc(MAXN * sizeof(point));
 range *r = (range*)malloc(MAXN * sizeof(range));
 for (int i=0; i<n; ++i) {
  r[i].s = min(row[i], col[i]);
  r[i].e = max(row[i], col[i]);
 }
 vector<range> R = parseRanges(r, n);
 ll res = find(R, k, m);

 // do the same for other diagonal
 // for (int i=0; i<n; ++i) {
 //  r[i].s = min(p[i].y,m-p[i].x-1);
 //  r[i].e = max(p[i].y,m-p[i].x-1);
 // }
 // R = parseRanges(r, n);
 //res = min(res, find(R, k, m));
 return res;
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
