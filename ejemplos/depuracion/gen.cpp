#include <cstdlib>
#include <iostream>
#include <random>
using namespace std;
int main(int argc, char** argv) {
    if (argc != 2) return 1;
    unsigned semilla = strtoul(argv[1], nullptr, 10);
    if (semilla == 1) { // caso borde deliberado, antes de los aleatorios
        cout << "3\n-5 -2 -7\n";
        return 0;
    }
    mt19937 rng(semilla);
    int n = uniform_int_distribution<int>(1, 8)(rng);
    cout << n << "\n";
    for (int i = 0; i < n; i++)
        cout << uniform_int_distribution<int>(-10, 10)(rng) << (i+1 == n ? '\n' : ' ');
}
