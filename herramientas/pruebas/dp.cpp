#include <bits/stdc++.h>
using namespace std;
#include "mochila_0_1.inc"
#include "subsecuencia_creciente_mas_larga_lis.inc"
#include "dp_con_bitmask.inc"
#include "convex_hull_trick_li_chao.inc"
vector<long long> G_pre;                        // datos del caso, visibles para cost()
namespace KN {
#include "optimizacion_de_knuth.inc"
long long cost(int l, int r) { return G_pre[r+1] - G_pre[l]; }
}
namespace DC {
#include "optimizacion_divide_y_venceras_dp.inc"
long long cost(int k, int j) { long long s = G_pre[j+1] - G_pre[k]; return s * s; }  // convexo: cumple QI
}
mt19937 rng(31337); int ri(int a,int b){return uniform_int_distribution<int>(a,b)(rng);}
int fallos=0; void chk(bool ok,const string&q){if(!ok){printf("  FALLO: %s\n",q.c_str());fallos++;}}
int main(){
    // ── Mochila vs todos los subconjuntos
    for(int it=0; it<400; it++){
        int n=ri(1,12), C=ri(0,25);
        vector<int> p(n), v(n);
        for(int i=0;i<n;i++){ p[i]=ri(1,12); v[i]=ri(0,20); }
        int mejor=0;
        for(int m=0;m<(1<<n);m++){ int pp=0,vv=0;
            for(int i=0;i<n;i++) if(m>>i&1){pp+=p[i];vv+=v[i];}
            if(pp<=C) mejor=max(mejor,vv); }
        chk(mochila(p,v,C)==mejor, "mochila 0/1");
    }
    // ── LIS vs la DP O(n^2)
    for(int it=0; it<500; it++){
        int n=ri(1,40); vector<int> a(n); for(auto&x:a) x=ri(-20,20);
        vector<int> f(n,1); int mejor=0;
        for(int i=0;i<n;i++){ for(int j=0;j<i;j++) if(a[j]<a[i]) f[i]=max(f[i],f[j]+1);
            mejor=max(mejor,f[i]); }
        chk(lis(a)==mejor, "LIS estrictamente creciente");
    }
    // ── Asignación con bitmask vs todas las permutaciones
    for(int it=0; it<300; it++){
        int n=ri(1,7); vector<vector<int>> c(n, vector<int>(n));
        for(auto&f:c) for(auto&x:f) x=ri(0,50);
        vector<int> perm(n); iota(perm.begin(),perm.end(),0);
        int mejor=INT_MAX;
        do{ int s=0; for(int i=0;i<n;i++) s+=c[i][perm[i]]; mejor=min(mejor,s); }
        while(next_permutation(perm.begin(),perm.end()));
        chk(asignacion(c)==mejor, "DP con bitmask (asignacion)");
    }
    // ── Li Chao vs mínimo por fuerza bruta sobre todas las rectas
    for(int it=0; it<300; it++){
        int N=ri(1,60); LiChao lc(N); vector<pair<long long,long long>> rectas;
        for(int k=0,K=ri(1,15); k<K; k++){
            long long m=ri(-20,20), b=ri(-200,200);
            lc.add(m,b); rectas.push_back({m,b});
            for(int x=0;x<N;x++){
                long long mn=LLONG_MAX; for(auto[mm,bb]:rectas) mn=min(mn,mm*x+bb);
                chk(lc.query(x)==mn, "Li Chao query");
            }
        }
    }
    // Coordenadas dispersas, negativas, repetidas y singleton; comparar en x original.
    { LiChaoDiscreto lc({100,1000}); lc.add(1,0); lc.add(0,50);
      chk(lc.query(100)==50, "Li Chao regresion coordenadas originales"); }
    for(int it=0;it<300;it++){
        vector<long long> xs; for(int i=0,n=ri(1,30);i<n;i++) xs.push_back(100000LL*ri(-20,20)+ri(0,3));
        LiChaoDiscreto lc(xs); vector<pair<long long,long long>> lines;
        chk(lc.query(xs[0])==LLONG_MAX, "Li Chao vacio");
        for(int j=0;j<15;j++){
            long long m=ri(-20,20), b=ri(-1000,1000); lc.add(m,b); lines.push_back({m,b});
            for(long long x:xs){ long long expected=LLONG_MAX;
                for(auto [mm,bb]:lines) expected=min(expected,mm*x+bb);
                chk(lc.query(x)==expected, "Li Chao disperso vs fuerza bruta"); }
        }
    }
    // La desigualdad sola no garantiza Knuth: verificar el contraejemplo didáctico.
    {
        vector<int> a={0,0,17,0}, b={12,6,13,0};
        auto c=[&](int l,int r){ return a[l]+b[r]; };
        for(int i=0;i<4;i++) for(int j=i;j<4;j++) for(int k=j;k<4;k++) for(int l=k;l<4;l++)
            chk(c(i,k)+c(j,l)==c(i,l)+c(j,k), "contraejemplo cumple cuadrangulo");
        chk(c(2,2)>c(0,3), "contraejemplo viola inclusion");
        vector<vector<int>> d(4,vector<int>(4)), opt(4,vector<int>(4));
        for(int i=0;i<4;i++) opt[i][i]=i;
        for(int len=2;len<=4;len++) for(int l=0;l+len<=4;l++){
            int r=l+len-1; d[l][r]=INT_MAX;
            for(int k=l;k<r;k++) if(d[l][k]+d[k+1][r]+c(l,r)<d[l][r]){
                d[l][r]=d[l][k]+d[k+1][r]+c(l,r); opt[l][r]=k; }
        }
        chk(d[0][3]==13 && opt[0][3]<opt[0][2], "contraejemplo opt no monotono");
    }
    // ── Knuth vs la DP de intervalos O(n^3)
    for(int it=0; it<200; it++){
        int n=ri(1,25); vector<long long> a(n), pre(n+1,0);
        for(int i=0;i<n;i++){ a[i]=ri(0,20); pre[i+1]=pre[i]+a[i]; }
        KN::n=n; KN::dp.assign(n, vector<long long>(n,0)); KN::opt.assign(n, vector<long long>(n,0));
        G_pre = pre;
        KN::knuth();
        vector<vector<long long>> d(n, vector<long long>(n,0));
        for(int len=2;len<=n;len++) for(int l=0;l+len-1<n;l++){ int r=l+len-1; d[l][r]=LLONG_MAX;
            for(int k=l;k<r;k++) d[l][r]=min(d[l][r], d[l][k]+d[k+1][r]+pre[r+1]-pre[l]); }
        for(int l=0;l<n;l++) for(int r=l;r<n;r++) chk(KN::dp[l][r]==d[l][r], "Knuth vs O(n^3)");
    }
    // ── Divide y vencerás vs la DP O(capas * n^2)
    for(int it=0; it<200; it++){
        int n=ri(1,25), capas=ri(1,4);
        vector<long long> a(n), pre(n+1,0);
        for(int i=0;i<n;i++){ a[i]=ri(1,10); pre[i+1]=pre[i]+a[i]; }
        auto w=[&](int k,int j){ long long s=pre[j+1]-pre[k]; return s*s; };  // la misma función que DC::cost
        DC::n=n; G_pre = pre;
        DC::dp.assign(capas+1, vector<long long>(n, LLONG_MAX/4));
        for(int j=0;j<n;j++) DC::dp[1][j]=w(0,j);
        vector<vector<long long>> d(capas+1, vector<long long>(n, LLONG_MAX/4));
        for(int j=0;j<n;j++) d[1][j]=w(0,j);
        for(int i=2;i<=capas;i++){
            DC::solve(i,0,n-1,0,n-1);
            for(int j=0;j<n;j++) for(int k=0;k<=j;k++) d[i][j]=min(d[i][j], d[i-1][k]+w(k,j));
        }
        for(int i=1;i<=capas;i++) for(int j=0;j<n;j++)
            chk(DC::dp[i][j]==d[i][j], "Divide y venceras vs O(n^2)");
    }
    printf(fallos? "\n>>> %d FALLOS\n" : "\n>>> DP OK\n", fallos);
}
