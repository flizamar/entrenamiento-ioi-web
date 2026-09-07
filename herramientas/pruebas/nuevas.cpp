// Verifica las 9 fichas agregadas en la revisión de 2026, contra fuerza bruta.
// Además de comprobar que aciertan, comprueba dos límites que las propias
// fichas declaran: que la ternaria se rompe con mesetas en la ladera, y que
// el caso de Aliens con empates entre distintos k está realmente ejercitado.
#include <bits/stdc++.h>
using namespace std;
#include "kadane_subarreglo_de_suma_maxima.inc"
#include "recursion_y_memoizacion.inc"
#include "dp_de_intervalos.inc"
#include "teoria_de_juegos_posiciones_ganadoras.inc"
#include "inclusion_exclusion.inc"
#include "greedy_y_argumento_de_intercambio.inc"
#include "deque_monotona_ventana_deslizante.inc"
// la ficha de búsqueda ternaria declara f y g; aquí les damos cuerpo
vector<long long> VALLE;
long long f(long long x) { return VALLE[x]; }
double COEF_A, COEF_B;
double g(double x) { return COEF_A * (x - COEF_B) * (x - COEF_B); }
#include "busqueda_ternaria.inc"
namespace AL {
#include "aliens_trick_optimizacion_lagrangiana.inc"
}
mt19937 rng(2026);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
long long EVALS = 0;
int fallos = 0;
void chk(bool ok, const string& q) { if (!ok) { printf("FALLO: %s\n", q.c_str()); fallos++; } }

int main() {

    // ── Kadane: suma Y ubicación, contra todos los subarreglos
    for(int it=0; it<3000; it++){
        int n=ri(1,14), modo=ri(0,2);
        vector<int> a(n);
        for(auto&x:a) x = modo==0? ri(-20,20) : (modo==1? ri(-20,-1) : ri(0,20));
        long long mejor=LLONG_MIN;
        for(int i=0;i<n;i++){ long long s=0; for(int j=i;j<n;j++){ s+=a[j]; mejor=max(mejor,s);} }
        Tramo t = kadane(a);
        chk(t.suma==mejor, "Kadane: la suma maxima");
        long long comp=0; for(int i=t.ini;i<=t.fin;i++) comp+=a[i];
        chk(comp==t.suma, "Kadane: los indices no suman lo que dice");
        chk(0<=t.ini && t.ini<=t.fin && t.fin<n, "Kadane: indices fuera de rango");
    }
    { vector<int> u{-5}; chk(kadane(u).suma==-5, "Kadane n=1 negativo");
      vector<int> z{0,0,0}; chk(kadane(z).suma==0, "Kadane todos ceros"); }

    // ── fib memoizado contra iterativo
    { memo.assign(91,-1);
      vector<long long> f(91); f[0]=0; f[1]=1;
      for(int i=2;i<=90;i++) f[i]=f[i-1]+f[i-2];
      for(int i=0;i<=90;i++) chk(fib(i)==f[i], "fib memoizado");
      for(int i=0;i<=90;i++) chk(fib(i)==f[i], "fib con la memo caliente"); }

    // ── caminos en grilla contra DP bottom-up
    for(int it=0; it<300; it++){
        int n=ri(0,12), m=ri(0,12);
        memo2.assign(n+1, vector<long long>(m+1,-1));
        vector<vector<long long>> d(n+1, vector<long long>(m+1,0));
        for(int i=0;i<=n;i++) for(int j=0;j<=m;j++)
            d[i][j] = (i==0||j==0)? 1 : d[i-1][j]+d[i][j-1];
        chk(caminos(n,m)==d[n][m], "caminos en grilla");
    }

    // ── juntar montones contra recursion exhaustiva
    for(int it=0; it<800; it++){
        int n=ri(1,8); vector<long long> a(n); for(auto&x:a) x=ri(1,15);
        vector<long long> pre(n+1,0); for(int i=0;i<n;i++) pre[i+1]=pre[i]+a[i];
        vector<vector<long long>> vis(n, vector<long long>(n,-1));
        function<long long(int,int)> bruto=[&](int l,int r)->long long{
            if(l==r) return 0;
            long long& v=vis[l][r]; if(v>=0) return v;
            long long mejor=LLONG_MAX;
            for(int k=l;k<r;k++) mejor=min(mejor, bruto(l,k)+bruto(k+1,r));
            return v = mejor + pre[r+1]-pre[l]; };
        chk(juntar(a)==bruto(0,n-1), "juntar montones");
    }

    // ── juego de extremos contra minimax exhaustivo
    for(int it=0; it<800; it++){
        int n=ri(1,10); vector<long long> a(n); for(auto&x:a) x=ri(-15,15);
        vector<vector<long long>> vis(n, vector<long long>(n,LLONG_MIN));
        function<long long(int,int)> mm=[&](int l,int r)->long long{
            if(l>r) return 0;
            long long& v=vis[l][r]; if(v!=LLONG_MIN) return v;
            return v = max(a[l]-mm(l+1,r), a[r]-mm(l,r-1)); };
        chk(juego(a)==mm(0,n-1), "juego de extremos (diferencia)");
    }

    // ── posiciones ganadoras contra un minimax que simula la partida
    for(int it=0; it<600; it++){
        int n=ri(1,30), k=ri(1,4); set<int> ss;
        while((int)ss.size()<k) ss.insert(ri(1,6));
        vector<int> S(ss.begin(), ss.end());
        for(int mis=0; mis<2; mis++){
            auto g = ganadoras(n, S, mis);
            vector<int> memo_(n+1,-1);
            function<int(int)> gana_bruto=[&](int i)->int{
                if(memo_[i]>=0) return memo_[i];
                bool puede=false, alguno=false;
                for(int s:S) if(s<=i){ puede=true; if(!gana_bruto(i-s)) alguno=true; }
                if(!puede) return memo_[i] = mis? 1 : 0;   // sin movimientos
                return memo_[i] = alguno; };
            for(int i=0;i<=n;i++) chk((bool)g[i]==(bool)gana_bruto(i),
                mis? "posiciones ganadoras (misere)" : "posiciones ganadoras (normal)");
        }
    }
    // el patron que la ficha afirma: con S={1..k}, pierden los multiplos de k+1
    for(int k=1;k<=6;k++){ vector<int> S(k); iota(S.begin(),S.end(),1);
        auto g=ganadoras(60,S,false);
        for(int i=0;i<=60;i++) chk((bool)g[i] == (i%(k+1)!=0), "el patron de multiplos de k+1"); }

    // ── inclusion-exclusion contra conteo directo
    for(int it=0; it<2000; it++){
        long long n=ri(1,600); int k=ri(1,4); set<long long> ds;
        while((int)ds.size()<k) ds.insert(ri(2,40));
        vector<long long> d(ds.begin(), ds.end());
        long long bruto=0;
        for(long long x=1;x<=n;x++) for(long long q:d) if(x%q==0){ bruto++; break; }
        chk(divisibles_por_alguno(n,d)==bruto, "inclusion-exclusion");
    }
    // el guardian de desbordamiento del mcm, con primos grandes
    { vector<long long> d{999999937LL, 999999893LL, 999999883LL};
      long long r = divisibles_por_alguno(4000000000LL, d);
      long long esp = 4000000000LL/999999937LL + 4000000000LL/999999893LL + 4000000000LL/999999883LL;
      chk(r==esp, "inclusion-exclusion: el mcm que se pasa de n no debe aportar"); }

    // ── selección de actividades contra TODOS los subconjuntos
    for(int it=0; it<2000; it++){
        int n=ri(1,11); vector<pair<int,int>> v(n);
        for(auto&p:v){ int a=ri(0,12), b=ri(a,13); p={b,a}; }   // {fin, inicio}
        int mejor=0;
        for(int m=0;m<(1<<n);m++){
            vector<pair<int,int>> sel;
            for(int i=0;i<n;i++) if(m>>i&1) sel.push_back({v[i].second, v[i].first});
            sort(sel.begin(), sel.end());
            bool ok=true;
            for(size_t i=1;i<sel.size();i++) if(sel[i].first < sel[i-1].second) ok=false;
            if(ok) mejor=max(mejor,(int)sel.size());
        }
        chk(max_actividades(v)==mejor, "seleccion de actividades");
    }
    // ── espera total contra TODAS las permutaciones
    for(int it=0; it<1500; it++){
        int n=ri(1,7); vector<long long> d(n); for(auto&x:d) x=ri(1,20);
        vector<int> idx(n); iota(idx.begin(),idx.end(),0);
        long long mejor=LLONG_MAX;
        do { long long reloj=0,tot=0; for(int i:idx){ reloj+=d[i]; tot+=reloj; }
             mejor=min(mejor,tot); } while(next_permutation(idx.begin(),idx.end()));
        chk(espera_total(d)==mejor, "espera total (shortest job first)");
    }
    // ── ternaria entera sobre valles con laderas ESTRICTAS (el contrato de la ficha)
    long long total_evals=0; int pasos=0;
    for(int it=0; it<3000; it++){
        int n=ri(2,200), fondo=ri(0,n-1), plano=ri(0,3);
        VALLE.assign(n,0);
        long long v=ri(0,50); int fin_plano=min(n-1,fondo+plano);
        for(int i=fondo;i<=fin_plano;i++) VALLE[i]=v;
        long long c=v; for(int i=fondo-1;i>=0;i--) VALLE[i]= c += ri(1,9);   // ladera estricta
        c=v; for(int i=fin_plano+1;i<n;i++) VALLE[i]= c += ri(1,9);
        long long mn=*min_element(VALLE.begin(), VALLE.end());
        EVALS=0; long long pos=argmin_entero(0,n-1);
        total_evals+=EVALS; pasos++;
        chk(VALLE[pos]==mn, "ternaria entera: el minimo");
    }
    // ── el contraejemplo que la ficha promete que rompe el contrato
    // el límite que la ficha declara: con meseta en la LADERA se equivoca.
    // Se afirma como assertion para que la nota no pueda quedar mintiendo.
    { VALLE = {5,5,3,3,7,7};
      chk(VALLE[argmin_entero(0,5)] != 3,
          "la ficha dice que las mesetas en la ladera la rompen, pero acerto"); }
    // ── ternaria real contra el minimo analitico
    for(int it=0; it<2000; it++){
        COEF_A = ri(1,20); COEF_B = ri(-50,50) + ri(0,99)/100.0;
        double lo=COEF_B-ri(1,80), hi=COEF_B+ri(1,80);
        double r = argmin_real(lo,hi);
        chk(fabs(r-COEF_B) < 1e-6, "ternaria real vs minimo analitico");
    }
    printf("  evaluaciones de f por llamada: %.1f en promedio (rango hasta 200)\n",
           (double)total_evals/pasos);

    // ── máximo de ventana 1D contra fuerza bruta, barriendo TODO k
    for(int it=0; it<1500; it++){
        int n=ri(1,18), modo=ri(0,2);
        vector<int> a(n);
        for(auto&x:a) x = modo==0? ri(-9,9) : (modo==1? 7 : ri(0,1));  // modo 1: todos iguales
        for(int k=1;k<=n;k++){
            auto r = max_ventana(a,k);
            chk((int)r.size()==n-k+1, "max_ventana: cantidad de ventanas");
            for(int i=0;i+k<=n;i++){ int mx=a[i]; for(int j=i;j<i+k;j++) mx=max(mx,a[j]);
                chk(r[i]==mx, "max_ventana: el maximo"); }
        }
    }
    // ── máximo de submatriz k×k contra fuerza bruta
    for(int it=0; it<400; it++){
        int n=ri(1,8), m=ri(1,8), k=ri(1,min(n,m));
        vector<vector<int>> g(n, vector<int>(m));
        for(auto&f:g) for(auto&x:f) x=ri(-9,9);
        auto r = max_submatriz(g,k);
        for(int i=0;i+k<=n;i++) for(int j=0;j+k<=m;j++){
            int mx=g[i][j];
            for(int x=i;x<i+k;x++) for(int y=j;y<j+k;y++) mx=max(mx,g[x][y]);
            chk(r[i][j]==mx, "max_submatriz"); }
    }
    // ── Aliens trick contra la DP exacta O(n*k), forzando empates
    int conEmpates=0;
    for(int it=0; it<1200; it++){
        int n=ri(1,22), modo=ri(0,2);
        vector<long long> a(n);
        for(auto&x:a) x = modo==0? ri(0,3) : (modo==1? ri(0,1) : 0);  // muchos empates
        AL::n=n; AL::pre.assign(n+1,0);
        for(int i=0;i<n;i++) AL::pre[i+1]=AL::pre[i]+a[i];
        long long total=AL::pre[n];
        int k=ri(1,min(n,6));
        // DP exacta: dp[j][t] = mejor costo partiendo a[0..j) en t trozos
        vector<vector<long long>> dp(n+1, vector<long long>(k+1, LLONG_MAX/4));
        dp[0][0]=0;
        for(int j=1;j<=n;j++) for(int t=1;t<=k;t++) for(int i=0;i<j;i++)
            if(dp[i][t-1]<LLONG_MAX/8)
                dp[j][t]=min(dp[j][t], dp[i][t-1]+(AL::pre[j]-AL::pre[i])*(AL::pre[j]-AL::pre[i]));
        long long exacto = dp[n][k];
        // ¿hay empate entre distintos k? (para saber si el test es exigente)
        for(int t=1;t<=k;t++) if(t!=k && dp[n][t]==exacto) conEmpates++;
        long long lam_max = total*total + 1;
        chk(AL::aliens(k, lam_max)==exacto, "Aliens trick vs DP exacta O(n*k)");
    }
    printf("  (casos con empates genuinos entre distintos k: %d)\n", conEmpates);
    printf(fallos ? "\n>>> %d FALLOS\n" : "\n>>> las 9 fichas nuevas son correctas\n", fallos);
    return fallos ? 1 : 0;
}
