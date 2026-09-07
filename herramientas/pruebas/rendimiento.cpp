#include <bits/stdc++.h>
#include <chrono>
using namespace std;
#include "fenwick_bit.inc"
#include "segment_tree.inc"
#include "segment_tree_con_lazy_propagation.inc"
#include "sparse_table_rmq.inc"
#include "dsu_union_find.inc"
#include "dijkstra.inc"
#include "lca_con_binary_lifting.inc"
#include "criba_de_eratostenes.inc"
#include "subsecuencia_creciente_mas_larga_lis.inc"
#include "convex_hull_trick_li_chao.inc"
using Clock = chrono::high_resolution_clock;
double seg(function<void()> f){ auto t0=Clock::now(); f(); 
    return chrono::duration<double>(Clock::now()-t0).count(); }
mt19937 rng(1); int ri(int a,int b){return uniform_int_distribution<int>(a,b)(rng);}

void reporta(const string& nombre, const string& claim, vector<int> ns, function<void(int)> run){
    vector<double> ts;
    for(int n : ns) ts.push_back(seg([&]{ run(n); }));
    printf("  %-26s %-22s", nombre.c_str(), claim.c_str());
    for(size_t i=0;i<ns.size();i++) printf(" %7.3fs", ts[i]);
    // pendiente log-log entre el primero y el ultimo: t ~ n^k
    double k = log(ts.back()/ts[0]) / log((double)ns.back()/ns[0]);
    printf("   -> t ~ n^%.2f\n", k);
}
int main(){
    printf("  %-26s %-22s %8s %8s %8s\n", "estructura", "complejidad decl.", "1x", "4x", "16x");
    reporta("Fenwick (n add + n sum)", "O(n log n)", {200000,800000,3200000}, [](int n){
        Fenwick f(n); for(int i=0;i<n;i++) f.add(i, i&7); volatile long long s=0;
        for(int i=0;i<n;i++) s+=f.sum(0,i); });
    reporta("SegTree (n upd + n qry)", "O(n log n)", {200000,800000,3200000}, [](int n){
        vector<long long> a(n,1); SegTree t(n); t.build(a);
        for(int i=0;i<n;i++) t.update(i, i&7); volatile long long s=0;
        for(int i=0;i<n;i++) s+=t.query(0,i); });
    reporta("LazySeg (n upd + n qry)", "O(n log n)", {200000,800000,3200000}, [](int n){
        LazySeg t(n); for(int i=0;i<n;i++) t.update(0,i,1); volatile long long s=0;
        for(int i=0;i<n;i++) s+=t.query(0,i); });
    reporta("SparseTable (build)", "O(n log n)", {200000,800000,3200000}, [](int n){
        vector<int> a(n); for(int i=0;i<n;i++) a[i]=i*7919%1000003; SparseTable sp(a);
        volatile int s=0; for(int i=0;i<n;i++) s+=sp.query(0,i); });
    reporta("DSU (n unions)", "O(n alfa(n))", {200000,800000,3200000}, [](int n){
        DSU d(n); for(int i=1;i<n;i++) d.unite(i-1,i);
        volatile int s=0; for(int i=0;i<n;i++) s+=d.find(i); });
    reporta("LCA (build + n qry)", "O(n log n)", {100000,400000,1600000}, [](int n){
        vector<vector<int>> g(n); for(int v=1;v<n;v++){ int p=v-1-(v%1000==0?0:0); if(v>1) p=v/2;
            g[p].push_back(v); g[v].push_back(p);} LCA lc(g,0);
        volatile int s=0; for(int i=0;i<n;i++) s+=lc.lca(i, n-1-i); });
    reporta("Criba de Eratostenes", "O(n log log n)", {2000000,8000000,32000000}, [](int n){
        auto p=criba(n); volatile size_t s=p.size(); (void)s; });
    reporta("LIS", "O(n log n)", {200000,800000,3200000}, [](int n){
        vector<int> a(n); for(int i=0;i<n;i++) a[i]=(int)((long long)i*7919%1000003);
        volatile int s=lis(a); (void)s; });
    reporta("Li Chao (n add + n qry)", "O(n log n)", {100000,400000,1600000}, [](int n){
        LiChao lc(n); for(int i=0;i<n;i++) lc.add(i%100-50, i);
        volatile long long s=0; for(int i=0;i<n;i++) s+=lc.query(i); });
    reporta("Dijkstra (grafo disperso)", "O((V+E) log V)", {100000,400000,1600000}, [](int n){
        vector<vector<pair<int,int>>> g(n);
        for(int i=0;i+1<n;i++){ g[i].push_back({i+1, (i%13)+1}); g[i+1].push_back({i,(i%13)+1}); }
        for(int i=0;i<n;i+=3){ int j=(i*7919)%n; g[i].push_back({j,(i%7)+1}); g[j].push_back({i,(i%7)+1}); }
        auto d=dijkstra(0,g); volatile long long s=d[n-1]; (void)s; });
}
