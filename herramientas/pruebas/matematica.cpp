#include <bits/stdc++.h>
using namespace std;
#include "criba_de_eratostenes.inc"
#include "factorizacion.inc"
#include "euclides_extendido_gcd.inc"
#include "exponenciacion_rapida_modular.inc"
#include "coeficientes_binomiales.inc"
#include "convex_hull.inc"
#include "barrido_sweep_line.inc"
mt19937_64 rng(555); long long ri(long long a,long long b){return uniform_int_distribution<long long>(a,b)(rng);}
int fallos=0; void chk(bool ok,const string&q){if(!ok){printf("  FALLO: %s\n",q.c_str());fallos++;}}
bool esPrimo(long long x){ if(x<2) return false; for(long long i=2;i*i<=x;i++) if(x%i==0) return false; return true; }
int main(){
    // ── Criba vs primalidad ingenua
    for(int n : {0,1,2,3,10,100,1000,10000}){
        auto pr = criba(n); set<int> s(pr.begin(), pr.end());
        for(int i=0;i<=n;i++) chk((s.count(i)>0)==esPrimo(i), "criba n="+to_string(n));
    }
    // ── Factorización: el producto reconstruye n y todos los factores son primos
    for(int it=0; it<3000; it++){
        long long n=ri(1, 1000000);
        auto f=factorize(n); long long prod=1;
        for(auto[p,e]:f){ chk(esPrimo(p), "factorize: factor no primo");
            for(int i=0;i<e;i++) prod*=p; }
        chk(prod==n, "factorize: el producto no reconstruye n");
    }
    // ── ext_gcd: a*x + b*y == g == gcd(a,b)
    for(int it=0; it<3000; it++){
        long long a=ri(0,1000000), b=ri(0,1000000); long long x,y;
        long long g=ext_gcd(a,b,x,y);
        chk(g==std::gcd(a,b), "ext_gcd: g != gcd");
        chk(a*x+b*y==g, "ext_gcd: identidad de Bezout");
    }
    // ── power vs multiplicación repetida
    for(int it=0; it<2000; it++){
        long long b=ri(0,1000), e=ri(0,30), mod=ri(1,100000);
        long long r=1%mod; for(int i=0;i<e;i++) r=r*(b%mod)%mod;
        chk(power(b,e,mod)==r, "power");
    }
    chk(power(5,0,1)==0, "power con mod=1");
    // ── Pascal vs fórmula factorial
    { auto C=pascal(25,0);
      for(int n=0;n<=25;n++) for(int k=0;k<=n;k++){
          long double e=1; for(int i=0;i<k;i++) e=e*(n-i)/(i+1);
          chk(llabs((long long)llroundl(e)-C[n][k])==0, "pascal C(n,k)"); } }
    // ── Convex hull: todos los puntos quedan dentro y el borde es convexo
    for(int it=0; it<400; it++){
        int n=ri(3,12); vector<P> p(n);
        for(auto&q:p){ q.x=ri(-8,8); q.y=ri(-8,8); }
        auto h=convex_hull(p);
        if(h.size()>=3){
            for(size_t i=0;i<h.size();i++){       // giro consistente (antihorario)
                P a=h[i], b=h[(i+1)%h.size()], c=h[(i+2)%h.size()];
                chk(cross(a,b,c)>0, "convex hull: giro no estrictamente convexo"); }
            for(auto&q:p){                        // ningun punto queda fuera
                bool dentro=true;
                for(size_t i=0;i<h.size();i++){ P a=h[i], b=h[(i+1)%h.size()];
                    if(cross(a,b,q)<0) dentro=false; }
                chk(dentro, "convex hull: punto fuera del casco"); }
        }
    }
    // ── Barrido vs marcar la recta celda por celda
    for(int it=0; it<2000; it++){
        int n=ri(1,8); vector<pair<long long,long long>> iv;
        vector<char> cubierto(40,0);
        for(int i=0;i<n;i++){ long long l=ri(0,35), r=ri(l,38); iv.push_back({l,r});
            for(long long x=l;x<r;x++) cubierto[x]=1; }
        long long bruto=0; for(int x=0;x<40;x++) bruto+=cubierto[x];
        chk(union_length(iv)==bruto, "barrido: longitud de la union");
    }
    printf(fallos? "\n>>> %d FALLOS\n" : "\n>>> matematica y geometria OK\n", fallos);
}
