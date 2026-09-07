#include <bits/stdc++.h>
using namespace std;
int n = 1, LOG;
vector<vector<int>> ady(1), up;
#include "ruta_m21-enraizar.inc"
#include "ruta_m21-subarbol.inc"
#include "ruta_m21-lca-simple.inc"
#include "ruta_m21-diametro.inc"
void iterativo(){
#include "ruta_m21-iterativo.inc"
}
void tabla(){
#include "ruta_m21-lifting.inc"
}
mt19937 rng(8); int ri(int a,int b){return uniform_int_distribution<int>(a,b)(rng);}
int fallos=0; void chk(bool ok,const string&q){if(!ok){printf("  FALLO: %s\n",q.c_str());fallos++;}}
int main(){
    for(int it=0; it<600; it++){
        n=ri(1,40); ady.assign(n,{}); tam.assign(n,0); padre.assign(n,-1); prof.assign(n,0);
        vector<int> pref(n,0), pr(n,0);
        for(int v=1;v<n;v++){ int p = ri(0,1)? ri(0,v-1) : v-1; pref[v]=p; pr[v]=pr[p]+1;
            ady[p].push_back(v); ady[v].push_back(p); }
        prof[0]=0; enraizar(0,-1); dfs(0,-1);
        for(int v=0;v<n;v++){ chk(prof[v]==pr[v], "prof"); chk(padre[v]==(v?pref[v]:-1), "padre"); }
        // tam contra conteo directo
        for(int v=0;v<n;v++){ int c=0; for(int u=0;u<n;u++){ int x=u; bool baja=false;
                while(true){ if(x==v){baja=true;break;} if(x==0) break; x=pref[x]; } if(baja) c++; }
            chk(tam[v]==c, "tam de subarbol"); }
        // La alternativa publicada debe producir los mismos resultados.
        auto padres_rec = padre, prof_rec = prof, tam_rec = tam;
        iterativo();
        chk(padre == padres_rec && prof == prof_rec && tam == tam_rec, "iterativo contra recursivo");
        // diámetro contra todos-contra-todos
        int mejor=0;
        for(int i=0;i<n;i++){ vector<int> d(n,-1); queue<int>q;q.push(i);d[i]=0;
            while(!q.empty()){int u=q.front();q.pop();for(int v:ady[u])if(d[v]<0){d[v]=d[u]+1;q.push(v);}}
            for(int j=0;j<n;j++){ mejor=max(mejor,d[j]);
                int ancestro=lca_simple(i,j);
                chk(prof[i]+prof[j]-2*prof[ancestro]==d[j], "distancia con LCA simple contra BFS"); } }
        chk(mas_lejano(mas_lejano(0).first).second==mejor, "diametro del modulo 21");
        // tabla de binary lifting: up[k][v] es el ancestro a 2^k pasos
        LOG=1; while((1<<LOG)<max(n,2)) LOG++;
        up.assign(LOG, vector<int>(n,0));
        for(int v=0;v<n;v++) up[0][v] = (v==0? 0 : pref[v]);
        tabla();
        for(int v=0;v<n;v++) for(int k=0;k<LOG;k++){
            int x=v; for(int s=0;s<(1<<k);s++) x = (x==0? 0 : pref[x]);
            chk(up[k][v]==x, "tabla de binary lifting"); }
    }
    n = 200000; ady.assign(n, {});
    for(int v=1;v<n;v++){ady[v-1].push_back(v);ady[v].push_back(v-1);}
    iterativo();
    chk(tam[0]==n && tam[n-1]==1 && prof[n-1]==n-1 && padre[n-1]==n-2,
        "arbol camino de 200000 nodos sin recursion");
    printf(fallos? "\n>>> %d FALLOS\n" : "\n>>> el codigo del modulo 21 es correcto\n", fallos);
    return fallos ? 1 : 0;
}
