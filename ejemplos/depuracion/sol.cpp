#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
// Contrato: 1 <= n <= 100000, -1000000000 <= a[i] <= 1000000000.
int main() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (auto& x : a) cin >> x;
    long long actual = a[0], mejor = a[0];
    for (int i = 1; i < n; i++) {
        actual = max(a[i], actual + a[i]);
        mejor = max(mejor, actual);
    }
    cout << mejor << "\n";
}
