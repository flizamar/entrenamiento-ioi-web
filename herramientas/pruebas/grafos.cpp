#include <bits/stdc++.h>
using namespace std;
#include "dsu_union_find.inc"
#include "dijkstra.inc"
#include "bellman_ford.inc"
#include "floyd_warshall.inc"
#include "kruskal_mst.inc"
#include "prim_mst.inc"
#include "orden_topologico_kahn.inc"
#include "scc_tarjan.inc"
mt19937 rng(999);
int ri(int a,int b){ return uniform_int_distribution<int>(a,b)(rng); }
int fallos=0;
void chk(bool ok,const string&q){ if(!ok){ printf("  FALLO: %s\n",q.c_str()); fallos++; } }
const long long BIG = 1e18;

int main(){
    // ── Dijkstra / Bellman-Ford / Floyd coinciden entre sí y con Floyd de referencia
    for(int it=0; it<400; it++){
        int n=ri(1,12);
        vector<vector<pair<int,int>>> g(n); vector<Edge> es;
        vector<vector<long long>> d(n, vector<long long>(n, BIG));
        for(int i=0;i<n;i++) d[i][i]=0;
        for(int e=0,m=ri(0,25); e<m; e++){
            int u=ri(0,n-1), v=ri(0,n-1), w=ri(0,20);
            g[u].push_back({v,w}); es.push_back({u,v,(long long)w});
            d[u][v]=min(d[u][v],(long long)w);
        }
        auto dref=d; floyd(dref);
        auto dij = dijkstra(0,g);
        vector<long long> bf; bool ok = bellman_ford(n,0,es,bf);
        chk(ok, "Bellman-Ford dice ciclo negativo con pesos >= 0");
        for(int v=0; v<n; v++){
            long long esperado = dref[0][v];
            chk(dij[v]==esperado || (dij[v]>=BIG && esperado>=BIG), "Dijkstra vs Floyd");
            chk(bf[v]==esperado || (bf[v]>=BIG && esperado>=BIG), "Bellman-Ford vs Floyd");
        }
    }
    // ── Bellman-Ford detecta ciclos negativos y acierta con pesos negativos
    for(int it=0; it<300; it++){
        int n=ri(2,9); vector<Edge> es;
        vector<vector<long long>> d(n, vector<long long>(n, BIG));
        for(int i=0;i<n;i++) d[i][i]=0;
        for(int e=0,m=ri(1,15); e<m; e++){
            int u=ri(0,n-1), v=ri(0,n-1), w=ri(-8,15);
            es.push_back({u,v,(long long)w}); d[u][v]=min(d[u][v],(long long)w);
        }
        // ¿hay ciclo negativo alcanzable desde 0? referencia: Floyd + d[i][i]<0 alcanzable
        auto dref=d; floyd(dref);
        bool negref=false;
        for(int i=0;i<n;i++) if(dref[i][i]<0 && dref[0][i]<BIG) negref=true;
        vector<long long> bf; bool ok=bellman_ford(n,0,es,bf);
        chk(ok == !negref, "Bellman-Ford deteccion de ciclo negativo");
        if(ok) for(int v=0;v<n;v++)
            chk(bf[v]==dref[0][v] || (bf[v]>=BIG && dref[0][v]>=BIG), "Bellman-Ford con pesos negativos");
    }
    // ── Kruskal vs Prim
    for(int it=0; it<400; it++){
        int n=ri(1,10);
        vector<array<long long,3>> es; vector<vector<pair<int,int>>> g(n);
        for(int e=0,m=ri(0,20); e<m; e++){
            int u=ri(0,n-1), v=ri(0,n-1), w=ri(1,30);
            es.push_back({(long long)w,(long long)u,(long long)v});
            g[u].push_back({v,w}); g[v].push_back({u,w});
        }
        DSU chk_d(n); int comps=n;
        for(auto&e:es) if(chk_d.unite((int)e[1],(int)e[2])) comps--;
        long long k=kruskal(n,es), p=prim(n,g);
        if(comps==1) chk(k==p, "Kruskal vs Prim (conexo)");
        else chk(p==-1, "Prim devuelve -1 si no es conexo");
    }
    // ── Toposort: orden valido y detecta ciclos
    for(int it=0; it<400; it++){
        int n=ri(1,12); vector<vector<int>> g(n); bool ciclo=ri(0,1);
        vector<int> perm(n); iota(perm.begin(),perm.end(),0); shuffle(perm.begin(),perm.end(),rng);
        for(int e=0,m=ri(0,20); e<m; e++){
            int i=ri(0,n-1), j=ri(0,n-1);
            if(!ciclo){ if(i==j) continue; if(perm[i]>perm[j]) swap(i,j); }
            g[i].push_back(j);
        }
        auto ord=toposort(g);
        if(!ord.empty() || n==0){
            chk((int)ord.size()==n, "toposort tamano");
            vector<int> pos(n); for(int i=0;i<n;i++) pos[ord[i]]=i;
            for(int u=0;u<n;u++) for(int v:g[u]) chk(pos[u]<pos[v], "toposort orden valido");
        } else {
            // dice que hay ciclo: verificar con DFS de colores
            vector<int> col(n,0); bool hay=false;
            function<void(int)> dfs=[&](int u){ col[u]=1;
                for(int v:g[u]){ if(col[v]==1) hay=true; else if(!col[v]) dfs(v); } col[u]=2; };
            for(int i=0;i<n;i++) if(!col[i]) dfs(i);
            chk(hay, "toposort dijo ciclo pero no lo hay");
        }
    }
    // ── SCC: misma componente <=> mutuamente alcanzables
    for(int it=0; it<300; it++){
        int n=ri(1,10); SCC s(n); vector<vector<int>> adj(n);
        for(int e=0,m=ri(0,25); e<m; e++){ int u=ri(0,n-1), v=ri(0,n-1);
            s.add(u,v); adj[u].push_back(v); }
        s.run();
        vector<vector<char>> alc(n, vector<char>(n,0));
        for(int i=0;i<n;i++){ vector<char> vis(n,0); queue<int>q; q.push(i); vis[i]=1;
            while(!q.empty()){int u=q.front();q.pop(); alc[i][u]=1; for(int v:adj[u]) if(!vis[v]){vis[v]=1;q.push(v);} } }
        for(int i=0;i<n;i++) for(int j=0;j<n;j++)
            chk((s.comp[i]==s.comp[j]) == (alc[i][j] && alc[j][i]), "SCC componentes");
        // Tarjan numera en orden topologico inverso de la condensacion
        for(int u=0;u<n;u++) for(int v:adj[u]) if(s.comp[u]!=s.comp[v])
            chk(s.comp[u] > s.comp[v], "SCC ids en orden topologico inverso");
    }
    printf(fallos? "\n>>> %d FALLOS\n" : "\n>>> todos los grafos OK\n", fallos);
}
