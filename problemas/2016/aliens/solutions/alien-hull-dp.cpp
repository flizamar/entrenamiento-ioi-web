#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

#define MAXN 3002
#define ll long long
#define INF ~(1LL<<63)
struct range {
 int s, e;

 friend bool operator < (const range &a, const range &b) {
  if (a.s < b.s) return true;
  if (a.s == b.s) {
   if (a.e > b.e) return true; else return false;
  } else
   return false;
 }
};

struct point {
 int x, y;
};

int M, N, K;
int NN, KK;

range a[MAXN];
point ps[MAXN];
vector<range> b;
ll dp[MAXN][3002];
pair<ll,ll> L[MAXN];
int len, cur;

double getx(ll m1, ll c1, ll m2, ll c2) {
 return (c2-c1) / (double)(m1-m2);
}

void addline(ll m, ll c) {
 while (len >= 2) {
  double a = getx(m, c, L[len-1].first, L[len-1].second);
  double b = getx(m, c, L[len-2].first, L[len-2].second);
  if (a > b) break;
  len--;
 }
 L[len++] = make_pair(m,c);
}

ll getmin(ll x) {
 if (cur >= len) cur = len-1;
 if (len==0) return -1;
 while (cur < len-1) {
  double a = getx(L[cur].first, L[cur].second, L[cur+1].first, L[cur+1].second);
  if (x > a)
   cur++;
  else
   break;
 }
 return L[cur].first * x + L[cur].second;
}

void resetline() {
 len = 0;
 cur = 0;
}

ll find() {
 sort(a, a+N);
 b.clear();
 b.push_back(a[0]);
 int cur = 0;
 for (int i=1; i<N; ++i) {
  if (a[cur].e < a[i].s) {
   cur = i;
   b.push_back(a[i]);
  } else if (a[cur].e < a[i].e) {
   cur = i;
   b.push_back(a[i]);
  }
 }
 NN = b.size();
 KK = min(K, NN);

 for (int i=0; i<NN; ++i)
  dp[i][1] = (ll)(b[i].e - b[0].s + 1) * (ll)(b[i].e - b[0].s + 1);

 for (int k=2; k<=KK; ++k) {
  resetline();
  for (int i=k-1; i<NN; ++i) {
   ll m, c;
   m = 2 * (1 - b[i].s);
   if (b[i-1].e < b[i].s)
    c = (ll)b[i].s * (ll)b[i].s + 1 - (ll)2*b[i].s;
   else
    c = -(ll)b[i-1].e*(ll)b[i-1].e + 2*(ll)b[i-1].e*(ll)b[i].s + 2*(ll)b[i].s - 2*(ll)b[i-1].e - 2*(ll)b[i].s;
   c += dp[i-1][k-1];
   addline(m, c);
   dp[i][k] = (ll)b[i].e * (ll)b[i].e + getmin((ll)b[i].e);
  }
 }
 ll ans = INF;
 for (int k=1; k<=KK; ++k) ans = min(ans, dp[NN-1][k]);
 return ans;
}

long long take_photos(int n, int m, int k, vector<int> row, vector<int> col) {
 ll ans;
 N = n;
 M = m;
 K = k;
 for (int i=0; i<N; ++i) ps[i].y = row[i], ps[i].x = col[i];

 for (int i=0; i<N; ++i)
  if (ps[i].y <= ps[i].x) {
   a[i].s = ps[i].y;
   a[i].e = ps[i].x;
  } else {
   a[i].s = ps[i].x;
   a[i].e = ps[i].y;
  }

 ans = find();

 // for (int i=0; i<N; ++i)
 //  if ((M-ps[i].y-1) <= ps[i].x) {
 //   a[i].s = M - ps[i].x - 1;
 //   a[i].e = ps[i].y;
 //  } else {
 //   a[i].s = ps[i].y;
 //   a[i].e = M - ps[i].x - 1;
 //  }
 // ans = min(ans, find());
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
