#include "suma_maxima.h"
#include <algorithm>
// Contrato: a no vacío; valores y sumas dentro de long long.
long long suma_maxima(const std::vector<long long>& a) {
    long long actual = a[0], mejor = a[0];
    for (int i = 1; i < (int)a.size(); i++) {
        actual = std::max(a[i], actual + a[i]);
        mejor = std::max(mejor, actual);
    }
    return mejor;
}
