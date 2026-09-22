#include <bits/stdc++.h>
using namespace std;
#include "heavy_light_decomposition.inc"
#include "centroid_decomposition.inc"
#include "segment_tree_persistente.inc"
mt19937 rng(77); int ri(int a,int b){return uniform_int_distribution<int>(a,b)(rng);}
int fallos=0; void chk(bool ok,const string&q){if(!ok){printf("  FALLO: %s\n",q.c_str());fallos++;}}
int main(){
    // ── HLD: suma sobre el camino vs camino calculado a mano
    for(int it=0; it<300; it++){
        int n=ri(1,30); HLD h(n); vector<vector<int>> g(n); vector<int> par(n,-1), prof(n,0);
        for(int v=1;v<n;v++){ int p=ri(0,v-1); par[v]=p; prof[v]=prof[p]+1;
            h.add(p,v); g[p].push_back(v); g[v].push_back(p); }
        h.build(0);
        vector<long long> val(n);
        for(int i=0;i<n;i++){ val[i]=ri(-20,20); h.update(i,val[i]); }
        for(int q=0;q<40;q++){
            int a=ri(0,n-1), b=ri(0,n-1), x=a, y=b; long long s=0;
            vector<int> ca, cb;
            while(prof[x]>prof[y]){ ca.push_back(x); x=par[x]; }
            while(prof[y]>prof[x]){ cb.push_back(y); y=par[y]; }
            while(x!=y){ ca.push_back(x); cb.push_back(y); x=par[x]; y=par[y]; }
            for(int u:ca) s+=val[u]; for(int u:cb) s+=val[u]; s+=val[x];
            chk(h.query_path(a,b)==s, "HLD query_path");
        }
        // y tras actualizar
        for(int k=0;k<10;k++){ int i=ri(0,n-1); val[i]=ri(-20,20); h.update(i,val[i]); }
        int a=ri(0,n-1), b=ri(0,n-1), x=a, y=b; long long s=0; vector<int> ca, cb;
        while(prof[x]>prof[y]){ca.push_back(x);x=par[x];} while(prof[y]>prof[x]){cb.push_back(y);y=par[y];}
        while(x!=y){ca.push_back(x);cb.push_back(y);x=par[x];y=par[y];}
        for(int u:ca)s+=val[u]; for(int u:cb)s+=val[u]; s+=val[x];
        chk(h.query_path(a,b)==s, "HLD query_path tras update");
    }
    // ── Centroid: el árbol de centroides tiene profundidad O(log n) y es un árbol válido
    for(int it=0; it<200; it++){
        int n=ri(1,60); CentroidDecomp cd(n); vector<vector<int>> g(n);
        for(int v=1;v<n;v++){ int p = ri(0,1)? ri(0,v-1) : v-1;   // mezcla de aleatorio y camino
            cd.add(p,v); g[p].push_back(v); g[v].push_back(p); }
        cd.run();
        int raices=0; for(int i=0;i<n;i++) if(cd.cpar[i]==-1) raices++;
        chk(raices==1, "centroid: exactamente una raiz");
        for(int i=0;i<n;i++){ int d=0, u=i; while(cd.cpar[u]!=-1){ u=cd.cpar[u]; d++;
                chk(d<=n, "centroid: ciclo en el arbol de centroides"); if(d>n) break; }
            int lim=1; while((1<<lim) < n+1) lim++;
            chk(d <= lim+2, "centroid: profundidad O(log n)"); }
    }
    // ── Segment tree persistente: cada versión conserva su historia
    for(int it=0; it<200; it++){
        int n=ri(1,20); PersistentSeg ps(n);
        vector<int> raiz; vector<vector<long long>> hist;
        raiz.push_back(ps.build(0,n-1)); hist.push_back(vector<long long>(n,0));
        for(int v=1; v<=15; v++){
            int p=ri(0,n-1); long long add=ri(-10,10);
            raiz.push_back(ps.update(raiz.back(),0,n-1,p,add));
            auto h=hist.back(); h[p]+=add; hist.push_back(h);
        }
        for(int v=0; v<(int)raiz.size(); v++)
            for(int q=0;q<20;q++){ int l=ri(0,n-1), r=ri(l,n-1); long long s=0;
                for(int i=l;i<=r;i++) s+=hist[v][i];
                chk(ps.query(raiz[v],0,n-1,l,r)==s, "PersistentSeg query por version"); }
    }
    printf(fallos? "\n>>> %d FALLOS\n" : "\n>>> arboles/persistente OK\n", fallos);
}
