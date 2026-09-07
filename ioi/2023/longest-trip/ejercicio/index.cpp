#include "longesttrip.h"
#include <vector>
#include <deque>
#include <iostream>
using namespace std;

// ===========================================================================
//  EJERCICIO GUIADO — Longest Trip (IOI 2023)        [lee GUIA.md]
//  Solo implementas longest_trip(). are_connected() la PROVEE el grader:
//  solo la llamas, no la declares ni la definas.
// ===========================================================================

// Mientras pruebas, deja DEBUG en 1. Antes de "enviar", ponlo en 0:
// las consultas de verificacion TAMBIEN cuentan en el presupuesto del juez.
#define DEBUG 1

// ---- Auto-verificador (ETAPA 2) -------------------------------------------
// Revisa que 'camino' sea un viaje valido: cada par consecutivo conectado.
// Imprime SIEMPRE a cerr (stderr), nunca a cout: cout es el canal del protocolo.
void verificar(const vector<int>& camino) {
#if DEBUG
    int rotos = 0;
    for (size_t i = 0; i + 1 < camino.size(); i++) {
        if (!are_connected({camino[i]}, {camino[i + 1]})) {
            cerr << "  [BUG] " << camino[i] << " y " << camino[i + 1]
                 << " NO estan conectados\n";
            rotos++;
        }
    }
    cerr << "  [debug] longitud=" << camino.size()
         << "  pares_rotos=" << rotos << "\n";
#endif
}

vector<int> longest_trip(int N, int D) {
    (void)D;  // en este ejercicio no usamos la densidad D todavia

    // ===================== ETAPA 1 =====================
    // Construye un viaje extendiendolo SOLO por el final:
    // para cada nodo nuevo v, pregunta si conecta con el ultimo del camino.
    deque<int> camino = {0};
    for (int v = 1; v < N; v++) {
        if (are_connected({camino.back()}, {v})) {
            camino.push_back(v);
        }
        // ===================== ETAPA 3 (TODO) =====================
        // Si no conecta por el final, intenta extender por el FRENTE.
        // Descomenta y completa:
        //
        // else if (are_connected({camino.front()}, {v})) {
        //     camino.push_front(v);
        // }
    }

    vector<int> ans(camino.begin(), camino.end());

    // ===================== ETAPA 2 (TODO) =====================
    // Descomenta para auto-verificar tu respuesta antes de devolverla.
    // verificar(ans);

    return ans;
}
