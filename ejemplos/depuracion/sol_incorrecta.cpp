#include <algorithm>
#include <iostream>
using namespace std;
// Error intencional para el ejercicio: permite elegir un subarreglo vacío.
int main() {
    int n;
    cin >> n;
    long long actual = 0, mejor = 0;
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;
        actual = max(0LL, actual + x);
        mejor = max(mejor, actual);
    }
    cout << mejor << "\n";
}
