#include <bits/stdc++.h>
#define MP make_pair
#define PB push_back
#define int long long
#define st first
#define nd second
#define rd third
#define FOR(i, a, b) for(int i =(a); i <=(b); ++i)
#define RE(i, n) FOR(i, 1, n)
#define FORD(i, a, b) for(int i = (a); i >= (b); --i)
#define REP(i, n) for(int i = 0;i <(n); ++i)
#define VAR(v, i) __typeof(i) v=(i)
#define FORE(i, c) for(VAR(i, (c).begin()); i != (c).end(); ++i)
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())
using namespace std;
template<typename TH> void _dbg(const char* sdbg, TH h) { cerr<<sdbg<<"="<<h<<"\n"; }
template<typename TH, typename... TA> void _dbg(const char* sdbg, TH h, TA... t) {
  while(*sdbg != ',')cerr<<*sdbg++; cerr<<"="<<h<<","; _dbg(sdbg+1, t...);
}
#ifdef LOCAL
#define debug(...) _dbg(#__VA_ARGS__, __VA_ARGS__)
#define debugv(x) {{cerr <<#x <<" = "; FORE(itt, (x)) cerr <<*itt <<", "; cerr <<"\n"; }}
#else
#define debug(...) (__VA_ARGS__)
#define debugv(x)
#define cerr if(0)cout
#endif
#define next ____next
#define prev ____prev
#define left ____left
#define hash ____hash
typedef long long ll;
typedef long double LD;
typedef pair<int, int> PII;
typedef pair<ll, ll> PLL;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<ll> VLL;
typedef vector<pair<int, int> > VPII;
typedef vector<pair<ll, ll> > VPLL;
 
template<class C> void mini(C&a4, C b4){a4=min(a4, b4); }
template<class C> void maxi(C&a4, C b4){a4=max(a4, b4); }
template<class T1, class T2>
ostream& operator<< (ostream &out, pair<T1, T2> pair) { return out << "(" << pair.first << ", " << pair.second << ")";}
template<class A, class B, class C> struct Triple { A first; B second; C third;
  bool operator<(const Triple& t) const { if (st != t.st) return st < t.st; if (nd != t.nd) return nd < t.nd; return rd < t.rd; } };
template<class T> void ResizeVec(T&, vector<int>) {}
template<class T> void ResizeVec(vector<T>& vec, vector<int> sz) {
  vec.resize(sz[0]); sz.erase(sz.begin()); if (sz.empty()) { return; }
  for (T& v : vec) { ResizeVec(v, sz); }
}
typedef Triple<int, int, int> TIII;
template<class A, class B, class C>
ostream& operator<< (ostream &out, Triple<A, B, C> t) { return out << "(" << t.st << ", " << t.nd << ", " << t.rd << ")"; }
template<class T> ostream& operator<<(ostream& out, vector<T> vec) { out<<"("; for (auto& v: vec) out<<v<<", "; return out<<")"; }
 
const int kDuzo = 1e7;
const int kInf = 2e9 + 1;
 
#undef int
long long plan_roller_coaster(std::vector<int> SS, std::vector<int> TT) {
#define int long long
  int n = SZ(SS);
//   for (auto x : SS) {
//     if (x > kDuzo) {
//       exit(1);
//     }
//   }
//   for (auto x : TT) {
//     if (x > kDuzo) {
//       exit(1);
//     }
//   }
   
  SS.PB(kInf);
  TT.PB(1);
  n++;
  VPII sort_s, sort_t;
  for (int i = 0; i < n; i++) {
    sort_s.PB({SS[i], i});
    sort_t.PB({TT[i], i});
  }
  sort(ALL(sort_s));
  sort(ALL(sort_t));
  VI wh_in_t(n);
  int res = 0;
  REP (i, n) {
    if (sort_s[i].st < sort_t[i].st) {
      res += sort_t[i].st - sort_s[i].st;
      sort_t[i].st = sort_s[i].st;
    }
    wh_in_t[sort_t[i].nd] = i;
  }
  debug(res);
  debug(sort_t);
  debug(wh_in_t);
  VI vis(n);
  //vector<VPII> cycles;
  vector<TIII> evs;
  int cc = 0;
  REP (ii, n) {
    if (vis[ii]) { continue; }
    int cur = ii;
    VPII cyc;
    do {
      debug(cur);
      vis[cur] = 1;
      evs.PB({sort_t[cur].st, 1, cc});
      evs.PB({sort_s[cur].st, 2, cc});
      //cyc.PB({sort_t[cur].st, sort_s[cur].st}); 
      cur = wh_in_t[sort_s[cur].nd];
    } while (cur != ii);
    cc++;
    //cycles.PB(cyc);
  }
  VI rep(n);
  iota(ALL(rep), 0);
  function<int(int)> Find = [&](int v) {
    if (rep[v] == v) { return v; }
    rep[v] = Find(rep[v]);
    return rep[v];
  };
  function<bool(int, int)> Union = [&](int a, int b) {
    debug(a, b);
    a = Find(a);
    b = Find(b);
    if (a == b) {
      return false;
    }
    rep[b] = a;
    return true;
  };
  vector<TIII> edges;
  sort(ALL(evs));
  debug(evs);
  multiset<int> alive;
  int last_who = -1;
  int last_pos = -1;
  for (auto p : evs) {
    int in = (p.nd == 1);
    if (in) {
      if (!alive.empty()) {
        Union(*alive.begin(), p.rd);
      } else {
        if (last_who != -1) {
          edges.PB({p.st - last_pos, p.rd, last_who});
        }
      }
      alive.insert(p.rd);
    } else {
      assert(alive.count(p.rd));
      alive.erase(alive.find(p.rd));
      last_who = p.rd;
      last_pos = p.st;
    }
  }
  sort(ALL(edges));
  for (auto e : edges) {
    if (Union(e.nd, e.rd)) {
      res += e.st;
    }
  }
  return res;
}
 
   
 
#undef int

//BEGIN CUT

int main() {
	int n, need_answer;
	scanf("%d%d", &n, &need_answer);
	std::vector<int> s(n), t(n);
	for (int i = 0; i < n; ++i)
		scanf("%d%d", &s[i], &t[i]);
	long long ans = plan_roller_coaster(s, t);
	
	// BEGIN SECRET
	puts("14e047d7a2907b9034950b074822b302");
	// END SECRET
	
	printf("%lld\n", ans);
	
	return 0;
}

//END CUT
