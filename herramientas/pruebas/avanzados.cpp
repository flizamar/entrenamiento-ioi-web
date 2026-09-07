#include <bits/stdc++.h>
using namespace std;
#include "puentes_y_puntos_de_articulacion.inc"
#include "matching_bipartito_kuhn.inc"
#include "flujo_maximo_ford_fulkerson.inc"
#include "diametro_de_un_arbol.inc"
#include "dp_en_arboles_rerooting.inc"
mt19937 rng(4242);
int ri(int a,int b){ return uniform_int_distribution<int>(a,b)(rng); }
int fallos=0;
void chk(bool ok,const string&q){ if(!ok){ printf("  FALLO: %s\n",q.c_str()); fallos++; } }

int comps(int n, vector<pair<int,int>> es, int quitarE=-1, int quitarV=-1){
    vector<vector<int>> g(n);
    for(int i=0;i<(int)es.size();i++){ if(i==quitarE) continue;
        auto[u,v]=es[i]; if(u==quitarV||v==quitarV) continue;
        g[u].push_back(v); g[v].push_back(u); }
    vector<char> vis(n,0); int c=0;
    for(int i=0;i<n;i++){ if(i==quitarV||vis[i]) continue; c++;
        queue<int>q;q.push(i);vis[i]=1;
        while(!q.empty()){int u=q.front();q.pop();for(int v:g[u])if(!vis[v]){vis[v]=1;q.push(v);}} }
    return c;
}
int main(){
    // ── Puentes y puntos de articulación vs quitar-y-contar-componentes
    for(int it=0; it<400; it++){
        int n=ri(1,9); vector<pair<int,int>> es; set<pair<int,int>> usadas;
        for(int e=0,m=ri(0,14); e<m; e++){ int u=ri(0,n-1), v=ri(0,n-1); if(u==v) continue;
            if(usadas.count({min(u,v),max(u,v)})) continue;      // sin multiaristas para el brute
            usadas.insert({min(u,v),max(u,v)}); es.push_back({u,v}); }
        BridgesAP b(n);
        for(int i=0;i<(int)es.size();i++) b.add(es[i].first, es[i].second, i);
        b.run();
        int base = comps(n,es);
        set<pair<int,int>> pts;
        for(auto[u,v]:b.bridges) pts.insert({min(u,v),max(u,v)});
        for(int i=0;i<(int)es.size();i++){
            bool esPuente = comps(n,es,i) > base;
            auto[u,v]=es[i];
            chk(esPuente == (pts.count({min(u,v),max(u,v)})>0), "puente");
        }
        for(int v=0; v<n; v++){
            int sinV = comps(n,es,-1,v);       // componentes del grafo sin v (v no cuenta)
            bool esAP = sinV > base;           // v aislado no aumenta; ojo con n=1
            chk(esAP == (b.isAP[v]==1), "punto de articulacion");
        }
    }
    // ── Kuhn vs flujo máximo (deben coincidir: matching = flujo en la red bipartita)
    for(int it=0; it<300; it++){
        int L=ri(1,7), R=ri(1,7);
        Kuhn k(L,R); MaxFlow mf(L+R+2); int s=L+R, t=L+R+1;
        for(int u=0;u<L;u++) mf.add(s,u,1);
        for(int v=0;v<R;v++) mf.add(L+v,t,1);
        for(int u=0;u<L;u++) for(int v=0;v<R;v++) if(ri(0,2)==0){ k.add(u,v); mf.add(u,L+v,1); }
        chk(k.max_matching() == mf.maxflow(s,t), "Kuhn vs MaxFlow");
    }
    // ── MaxFlow vs corte mínimo por fuerza bruta (todos los subconjuntos)
    for(int it=0; it<200; it++){
        int n=ri(2,7), s=0, t=n-1;
        vector<vector<long long>> c(n, vector<long long>(n,0));
        MaxFlow mf(n);
        for(int u=0;u<n;u++) for(int v=0;v<n;v++) if(u!=v && ri(0,2)==0){
            long long w=ri(1,9); c[u][v]+=w; mf.add(u,v,w); }
        long long mejor=LLONG_MAX;
        for(int mask=0; mask<(1<<n); mask++){
            if(!(mask&1) || (mask>>(n-1)&1)) continue;      // s en S, t fuera
            long long corte=0;
            for(int u=0;u<n;u++) if(mask>>u&1) for(int v=0;v<n;v++) if(!(mask>>v&1)) corte+=c[u][v];
            mejor=min(mejor,corte);
        }
        chk(mf.maxflow(s,t)==mejor, "MaxFlow == corte minimo");
    }
    // ── Diámetro vs todos-contra-todos
    for(int it=0; it<400; it++){
        int n=ri(1,30); vector<vector<int>> g(n);
        for(int v=1;v<n;v++){ int p=ri(0,v-1); g[p].push_back(v); g[v].push_back(p); }
        int mejor=0;
        for(int i=0;i<n;i++){ vector<int> d(n,-1); queue<int>q; q.push(i); d[i]=0;
            while(!q.empty()){int u=q.front();q.pop();for(int v:g[u])if(d[v]<0){d[v]=d[u]+1;q.push(v);}}
            for(int j=0;j<n;j++) mejor=max(mejor,d[j]); }
        chk(diametro(g,0)==mejor, "diametro");
    }
    // ── Rerooting (suma de distancias) vs BFS desde cada nodo
    for(int it=0; it<300; it++){
        int n=ri(1,25); Reroot r(n); vector<vector<int>> g(n);
        for(int v=1;v<n;v++){ int p=ri(0,v-1); r.add(p,v); g[p].push_back(v); g[v].push_back(p); }
        r.run(0);
        for(int i=0;i<n;i++){ vector<int> d(n,-1); queue<int>q;q.push(i);d[i]=0; long long s=0;
            while(!q.empty()){int u=q.front();q.pop(); s+=d[u]; for(int v:g[u])if(d[v]<0){d[v]=d[u]+1;q.push(v);}}
            chk(r.ans[i]==s, "rerooting suma de distancias"); }
    }
    printf(fallos? "\n>>> %d FALLOS\n" : "\n>>> avanzados OK\n", fallos);
}
