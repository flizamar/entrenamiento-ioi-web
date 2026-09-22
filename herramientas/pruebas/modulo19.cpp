// Verifica el código que publica la lección del módulo 19 (Fenwick, segment
// tree y los operadores de bits), no las fichas del catálogo.
#include <bits/stdc++.h>
using namespace std;

namespace BITS {
int k = 2, x = 13;
void modismos() {
#include "ruta_m19-bits.inc"
    assert(potencia == 4);              // 2^2
    assert(encendido == true);          // 13 = 1101, su bit 2 está encendido
}
}

namespace FEN {
#include "ruta_m19-fenwick.inc"
}

namespace SEG {
#include "ruta_m19-segment-tree.inc"
}

mt19937 rng(19);
int ri(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
int fallos = 0;
void chk(bool ok, const string& q) { if (!ok) { printf("  FALLO: %s\n", q.c_str()); fallos++; } }

int main() {
    BITS::modismos();
    // la tabla de operadores que muestra el módulo dice estos valores exactos
    chk((12 & 10) == 8,  "la tabla del modulo 19: 12 & 10");
    chk((12 | 10) == 14, "la tabla del modulo 19: 12 | 10");
    chk((1 << 3) == 8,   "la tabla del modulo 19: 1 << 3");
    chk((13 >> 2) == 3,  "la tabla del modulo 19: 13 >> 2");
    chk((6 & -6) == 2,   "el modulo 19 afirma que 6 & -6 vale 2");
    chk((12 & -12) == 4, "el modulo 19 afirma que 12 & -12 vale 4");

    // Fenwick de la lección (1-indexado) contra sumas por fuerza bruta
    for (int it = 0; it < 400; it++) {
        int n = ri(1, 40);
        FEN::n = n; FEN::bit.assign(n + 1, 0);
        vector<long long> a(n + 1, 0);
        for (int op = 0; op < 80; op++) {
            if (ri(0, 1)) {
                int i = ri(1, n); long long v = ri(-50, 50);
                a[i] += v; FEN::sumar(i, v);
            } else {
                int l = ri(1, n), r = ri(l, n), s = 0;
                for (int i = l; i <= r; i++) s += a[i];
                chk(FEN::prefijo(r) - FEN::prefijo(l - 1) == s, "Fenwick del modulo 19");
            }
        }
    }
    // segment tree de la lección contra mínimos por fuerza bruta
    for (int it = 0; it < 400; it++) {
        int n = ri(1, 40);
        SEG::n = n; SEG::t.assign(4 * n, 0);
        vector<long long> a(n);
        for (auto& x : a) x = ri(-99, 99);
        SEG::build(a, 1, 0, n - 1);
        for (int op = 0; op < 80; op++) {
            if (ri(0, 1)) {
                int i = ri(0, n - 1); long long v = ri(-99, 99);
                a[i] = v; SEG::update(1, 0, n - 1, i, v);
            } else {
                int l = ri(0, n - 1), r = ri(l, n - 1); long long mn = LLONG_MAX;
                for (int i = l; i <= r; i++) mn = min(mn, a[i]);
                chk(SEG::query(1, 0, n - 1, l, r) == mn, "segment tree del modulo 19");
            }
        }
    }
    printf(fallos ? "\n>>> %d FALLOS\n" : "\n>>> el codigo del modulo 19 es correcto\n", fallos);
    return fallos ? 1 : 0;
}
