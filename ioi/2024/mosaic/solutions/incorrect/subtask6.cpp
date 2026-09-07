#include "mosaic.h"
#include "bits/stdc++.h"
using namespace std;
const int mxN = 1e6 + 5;
int getLayer(int x, int y) {
    return min(x, y);
}
int n;
map<int, int> layers[mxN];    ///holding layers up to 3
int genNor(int x, int y) {
    return not (x or y);
}
void genLayers(vector<int> r, vector<int> c) {
    int n = r.size();
    for(int i = 0; i < n; i++) {
        layers[0][i] = r[i];
    }
    for(int i = 0; i < n; i++) {
        layers[i][0] = c[i];
    }
    for(int layer = 1; layer <= 2; layer++) {
        for(int i = layer; i < n; i++) {
            layers[layer][i] = genNor(layers[layer-1][i], layers[layer][i-1]);
            layers[i][layer] = genNor(layers[i-1][layer], layers[i][layer-1]);            
        }
    }
}
int getOrderInLayer(int x, int y) {
    int layer = getLayer(x, y);
    if(y == layer) {
        return n - x - 1;
    }   else {
        return (n - layer - 1) + (y - layer);
    }
}
vector<long long> mosaic(vector<int> X, vector<int> Y, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
    n = X.size();
    genLayers(X, Y);    ///generate all layers
    ///get all layer 3 values in order

    vector<int> layer3;
    for(int i = n-1; i >= 2; --i){
        layer3.push_back(layers[i][2]);
    }
    for(int i = 3; i < n; i++) {
        layer3.push_back(layers[2][i]);
    }

    vector<long long> ans;
    for(int i = 0; i < T.size(); i++) {
        int rx = T[i], ry = B[i];
        int cx = L[i], cy = R[i];
        int layer = getLayer(rx, cx);
        if(layer <= 2) {
            ans.push_back(layers[rx][cx]);
        }   else {
            int ord = getOrderInLayer(rx, cx);
            ans.push_back(layer3[ord+layer-2]);
        }
    }
    
    return ans;
}
