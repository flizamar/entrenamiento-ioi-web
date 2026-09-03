// point (x,y) -> range (min(y,x), max(y,x))  and  range (min(y,N-x-1), max(y,N-x-1))
// sort ranges
// DP function f(i,k) = min f(j,k-1) + (e(i)-s(j+1)+1)^2 - (max((e(j)-s(j+1)+1), 0))^2

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

#define ll long long
#define MAXN 100001
#define pb push_back
#define INFI (~(1LL<<63))
#define sq(x) ((x) * (x))

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
 double m, c;
};

ll find(vector<range> r, int KK) {
 int n =  r.size();
 KK = min(KK, n);
 ll dp[2][n];
 int cur = 1, tar=0;
 ll res;
 for (int i=0; i<n; ++i) dp[0][i] = (r[i].e-r[0].s+1) * (r[i].e-r[0].s+1);
 for (int k=2; k<=KK; ++k) {
  tar = cur; cur = (cur+1) % 2;
  for (int i=k-1; i<n; ++i) {
   res = INFI;
   for (int j=k-2; j<i; ++j)
    res = min(res, dp[cur][j] + sq(r[i].e - r[j+1].s + 1LL) - sq(max(r[j].e - r[j+1].s + 1LL, 0LL)));
   dp[tar][i] = res;
  }
 }
 return dp[tar][n-1];
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
 ll res = find(R, k);
 // for (int i=0; i<n; ++i) {
 //  r[i].s = min(p[i].y,m-p[i].x-1);
 //  r[i].e = max(p[i].y,m-p[i].x-1);
 // }
 // R = parseRanges(r, n);
 // res = min(res, find(R, k));
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
