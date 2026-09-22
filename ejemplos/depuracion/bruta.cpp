#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
// Enumeración O(n^2): se usa solo con casos pequeños.
int main() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (auto& x : a) cin >> x;
    long long mejor = a[0];
    for (int l = 0; l < n; l++) {
        long long suma = 0;
        for (int r = l; r < n; r++) {
            suma += a[r];
            mejor = max(mejor, suma);
        }
    }
    cout << mejor << "\n";
}
