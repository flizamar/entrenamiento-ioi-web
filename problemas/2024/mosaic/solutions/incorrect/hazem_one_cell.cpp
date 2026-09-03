#include "bits/stdc++.h"
#include "mosaic.h"
using namespace std;

vector<long long> mosaic(vector<int> X, vector<int> Y, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
    auto layer_index = [](int i, int j){
        return min(i, j);
    };
    auto cell_index = [](int i, int j){
        return ((i < j) - (i > j)) * (max(i, j) - min(i, j));
    };
    auto next_layer = [](vector<long long> l){
        if(l.size() < 3) return vector<long long>();
        int m = (int)l.size() / 2;
        l[m] = !(l[m-1] | l[m+1]);
        l.erase(l.begin() + m + 1);
        l.erase(l.begin() + m - 1);
        m--;
        for(int i = 1; i <= m; i++){
            l[m-i] = !(l[m-i] | l[m-i+1]);
            l[m+i] = !(l[m+i] | l[m+i-1]);
        }
        return l;
    };

    vector<long long> layer[4];
    layer[0] = {Y.rbegin(), Y.rend()};
    layer[0].insert(layer[0].end(), X.begin()+1, X.end());
    layer[1] = next_layer(layer[0]);
    layer[2] = next_layer(layer[1]);

    auto find_cell = [&](int i, int j){
        int li = layer_index(i, j);
        int ci = cell_index(i, j);
        int m = (int)layer[min(2, li)].size() / 2;
        return layer[min(2, li)][m + ci];
    };

    vector<long long> C(T.size());
    for(int q = 0; q < (int)C.size(); q++){
        auto [t, b, l, r] = tie(T[q], B[q], L[q], R[q]);
        C[q] = find_cell(t, l);
    }
    return C;
}
