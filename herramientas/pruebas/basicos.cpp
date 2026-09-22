#include <bits/stdc++.h>
using namespace std;
#include "bfs.inc"
#include "flood_fill.inc"
namespace BB {
#include "busqueda_binaria_clasica.inc"
}
using BB::buscar;
#include "dos_punteros_y_ventana_deslizante.inc"
#include "stack_monotono.inc"
mt19937 rng(2024); int ri(int a,int b){return uniform_int_distribution<int>(a,b)(rng);}
int fallos=0; void chk(bool ok,const string&q){if(!ok){printf("  FALLO: %s\n",q.c_str());fallos++;}}
int main(){
    // ── BFS vs Floyd sobre grafo no ponderado
    for(int it=0; it<400; it++){
        int n=ri(1,10); vector<vector<int>> g(n);
        vector<vector<int>> d(n, vector<int>(n, 1e9));
        for(int i=0;i<n;i++) d[i][i]=0;
        for(int e=0,m=ri(0,20); e<m; e++){ int u=ri(0,n-1), v=ri(0,n-1);
            g[u].push_back(v); d[u][v]=min(d[u][v],1); }
        for(int k=0;k<n;k++)for(int i=0;i<n;i++)for(int j=0;j<n;j++) d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
        auto dist=bfs(0,g);
        for(int v=0;v<n;v++) chk(dist[v]==(d[0][v]>=1e9? -1 : d[0][v]), "BFS distancias");
    }
    // ── Flood fill: contar regiones vs union-find sobre celdas
    for(int it=0; it<400; it++){
        int F=ri(1,8), C=ri(1,8); vector<string> m(F, string(C,'.'));
        for(int i=0;i<F;i++) for(int j=0;j<C;j++) if(ri(0,2)==0) m[i][j]='#';
        vector<int> p(F*C); iota(p.begin(),p.end(),0);
        function<int(int)> find=[&](int x){ return p[x]==x? x : p[x]=find(p[x]); };
        for(int i=0;i<F;i++) for(int j=0;j<C;j++) if(m[i][j]=='.'){
            if(i+1<F && m[i+1][j]=='.') p[find(i*C+j)]=find((i+1)*C+j);
            if(j+1<C && m[i][j+1]=='.') p[find(i*C+j)]=find(i*C+j+1); }
        set<int> raices;
        for(int i=0;i<F;i++) for(int j=0;j<C;j++) if(m[i][j]=='.') raices.insert(find(i*C+j));
        chk(regiones(m)==(int)raices.size(), "flood fill: numero de regiones");
    }
    // ── Búsqueda binaria vs búsqueda lineal
    for(int it=0; it<3000; it++){
        int n=ri(0,30); vector<int> a(n); for(auto&x:a) x=ri(-15,15);
        sort(a.begin(), a.end());
        int x=ri(-17,17); int pos=buscar(a,x);
        bool esta = find(a.begin(),a.end(),x)!=a.end();
        if(esta) chk(pos>=0 && a[pos]==x, "busqueda binaria: no encontro uno que esta");
        else     chk(pos==-1, "busqueda binaria: encontro uno que no esta");
    }
    // ── Ventana deslizante vs todos los subarreglos
    for(int it=0; it<2000; it++){
        int n=ri(1,18); vector<int> a(n); for(auto&x:a) x=ri(0,10);
        long long obj=ri(0,40);
        int mejor=INT_MAX;
        for(int i=0;i<n;i++){ long long s=0; for(int j=i;j<n;j++){ s+=a[j];
            if(s>=obj) { mejor=min(mejor,j-i+1); break; } } }
        int esperado = mejor==INT_MAX? -1 : mejor;
        chk(ventana(a,obj)==esperado, "ventana deslizante");
    }
    // ── Stack monótono vs búsqueda directa
    for(int it=0; it<2000; it++){
        int n=ri(0,25); vector<int> a(n); for(auto&x:a) x=ri(-8,8);
        auto r=next_greater(a);
        for(int i=0;i<n;i++){ int esp=-1; for(int j=i+1;j<n;j++) if(a[j]>a[i]){esp=j;break;}
            chk(r[i]==esp, "stack monotono: siguiente mayor"); }
    }
    // ── el comentario del ordenamiento dice la verdad
    { vector<pair<int,string>> v = {{80,"ana"},{95,"rocio"},{80,"belen"}};
      sort(v.begin(), v.end());
      chk(v[0].second=="ana"&&v[1].second=="belen"&&v[2].second=="rocio", "sort de pares: el comentario"); }
    printf(fallos? "\n>>> %d FALLOS\n" : "\n>>> basicos OK\n", fallos);
}
