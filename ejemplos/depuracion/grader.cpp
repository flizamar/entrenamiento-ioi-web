#include "suma_maxima.h"
#include <iostream>
// Adaptador didáctico de entrada/salida; no es un corrector oficial.
int main() {
    int n;
    std::cin >> n;
    std::vector<long long> a(n);
    for (auto& x : a) std::cin >> x;
    std::cout << suma_maxima(a) << "\n";
}
