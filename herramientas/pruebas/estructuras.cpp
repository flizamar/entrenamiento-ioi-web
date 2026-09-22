#include <bits/stdc++.h>
using namespace std;
// --- implementaciones tal como se publican ---
#include "fenwick_bit.inc"
#include "segment_tree.inc"
#include "segment_tree_con_lazy_propagation.inc"
#include "sparse_table_rmq.inc"
#include "dsu_union_find.inc"
#include "lca_con_binary_lifting.inc"
#include "trie.inc"

mt19937 rng(12345);
int ri(int a, int b){ return uniform_int_distribution<int>(a,b)(rng); }
int fallos = 0;
void chk(bool ok, const string& q){ if(!ok){ printf("  FALLO: %s\n", q.c_str()); fallos++; } }

int main(){
    // ── Fenwick vs prefijos por fuerza bruta
    for(int it=0; it<300; it++){
        int n = ri(1,40); vector<long long> a(n,0); Fenwick f(n);
        for(int op=0; op<80; op++){
            if(ri(0,1)){ int i=ri(0,n-1); long long v=ri(-50,50); a[i]+=v; f.add(i,v); }
            else { int l=ri(0,n-1), r=ri(l,n-1); long long s=0; for(int i=l;i<=r;i++) s+=a[i];
                   chk(f.sum(l,r)==s, "Fenwick sum(l,r)"); }
        }
    }
    // ── SegTree vs fuerza bruta
    for(int it=0; it<300; it++){
        int n=ri(1,40); vector<long long> a(n); for(auto&x:a) x=ri(-50,50);
        SegTree st(n); st.build(a);
        for(int op=0; op<80; op++){
            if(ri(0,1)){ int i=ri(0,n-1); long long v=ri(-50,50); a[i]=v; st.update(i,v); }
            else { int l=ri(0,n-1), r=ri(l,n-1); long long s=0; for(int i=l;i<=r;i++) s+=a[i];
                   chk(st.query(l,r)==s, "SegTree query"); }
        }
    }
    // ── LazySeg (range add / range sum) vs fuerza bruta
    for(int it=0; it<300; it++){
        int n=ri(1,40); vector<long long> a(n,0); LazySeg ls(n);
        for(int op=0; op<80; op++){
            int l=ri(0,n-1), r=ri(l,n-1);
            if(ri(0,1)){ long long v=ri(-50,50); for(int i=l;i<=r;i++) a[i]+=v; ls.update(l,r,v); }
            else { long long s=0; for(int i=l;i<=r;i++) s+=a[i];
                   chk(ls.query(l,r)==s, "LazySeg query"); }
        }
    }
    // ── SparseTable vs mínimo por fuerza bruta
    for(int it=0; it<300; it++){
        int n=ri(1,40); vector<int> a(n); for(auto&x:a) x=ri(-1000,1000);
        SparseTable sp(a);
        for(int q=0;q<60;q++){ int l=ri(0,n-1), r=ri(l,n-1);
            int mn=a[l]; for(int i=l;i<=r;i++) mn=min(mn,a[i]);
            chk(sp.query(l,r)==mn, "SparseTable query"); }
    }
    // ── DSU vs componentes por BFS
    for(int it=0; it<200; it++){
        int n=ri(1,25); DSU d(n); vector<vector<int>> g(n);
        for(int e=0;e<ri(0,40);e++){ int u=ri(0,n-1), v=ri(0,n-1); d.unite(u,v);
            g[u].push_back(v); g[v].push_back(u); }
        vector<int> comp(n,-1); int c=0;
        for(int i=0;i<n;i++) if(comp[i]<0){ queue<int>q; q.push(i); comp[i]=c;
            while(!q.empty()){int u=q.front();q.pop(); for(int v:g[u]) if(comp[v]<0){comp[v]=c;q.push(v);} } c++; }
        for(int i=0;i<n;i++) for(int j=0;j<n;j++)
            chk((d.find(i)==d.find(j)) == (comp[i]==comp[j]), "DSU find");
    }
    // ── LCA vs subida ingenua
    for(int it=0; it<200; it++){
        int n=ri(1,40); vector<vector<int>> g(n); vector<int> par(n,0), prof(n,0);
        for(int v=1;v<n;v++){ int p=ri(0,v-1); par[v]=p; prof[v]=prof[p]+1;
            g[p].push_back(v); g[v].push_back(p); }
        LCA lc(g, 0);
        for(int q=0;q<60;q++){
            int a=ri(0,n-1), b=ri(0,n-1), x=a, y=b;
            while(prof[x]>prof[y]) x=par[x];
            while(prof[y]>prof[x]) y=par[y];
            while(x!=y){ x=par[x]; y=par[y]; }
            chk(lc.lca(a,b)==x, "LCA lca");
            chk(lc.dist(a,b)==prof[a]+prof[b]-2*prof[x], "LCA dist");
        }
    }
    // ── Trie vs set
    for(int it=0; it<200; it++){
        Trie tr; set<string> s; vector<string> todas;
        for(int k=0;k<20;k++){ string w; for(int j=0,L=ri(1,5);j<L;j++) w+=char('a'+ri(0,3));
            tr.insert(w); s.insert(w); todas.push_back(w); }
        for(int q=0;q<40;q++){ string w; for(int j=0,L=ri(1,5);j<L;j++) w+=char('a'+ri(0,3));
            chk(tr.contains(w)==(s.count(w)>0), "Trie contains");
            bool pre=false; for(auto&t:s) if(t.compare(0,min(t.size(),w.size()),w)==0 && t.size()>=w.size()) pre=true;
            chk(tr.prefijo(w)==pre, "Trie prefijo"); }
    }
    printf(fallos? "\n>>> %d FALLOS\n" : "\n>>> todas las estructuras OK\n", fallos);
    return 0;
}
