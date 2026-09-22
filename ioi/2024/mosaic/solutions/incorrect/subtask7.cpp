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
vector<int> sums[3];
vector<int> arrays[3];
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
    ///Generate Prefix sums
    for(int layer = 0; layer < 3; layer++) {
        for(int i = n - 1; i >= layer; --i) {
            arrays[layer].push_back(layers[i][layer]);
        }
        for(int i = layer + 1; i < n; i++) {
            arrays[layer].push_back(layers[layer][i]);
        }
        for(int i = 0; i < arrays[layer].size(); i++) {
            if(i == 0) sums[layer].push_back(0);
            else sums[layer].push_back(sums[layer].back());
            sums[layer].back() += arrays[layer][i];
        }
    }
}
int getLayerIndex(int x, int y) {
    int layer = min(x, y);
    if(y == layer) {
        return n - x - 1;
    }   else {
        return (n - layer - 1) + (y - layer);
    }
}

vector<long long> mosaic(vector<int> X, vector<int> Y, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
    n = X.size();
    genLayers(X, Y);    ///generate all layers
    vector<long long> ans;
    for(int i = 0; i < T.size(); i++) {
        int rx = T[i], ry = B[i];
        int cx = L[i], cy = R[i];
        int total = 0;
        ///rx  == ry
        if(rx == 0) {
            int orda = getLayerIndex(rx,cx), ordb = getLayerIndex(ry, cy);
            total += sums[0][ordb] - sums[0][orda] + layers[0][cx];
            ans.push_back(total);
        }   else if(rx == 1) {
            while(cx <= cy && cx < 1) {
                total += layers[rx][cx++];
            }
            if(cx <= cy) {
                int orda = getLayerIndex(rx,cx), ordb = getLayerIndex(ry, cy);
                total += sums[1][ordb] - sums[1][orda] + layers[1][cx];
            }
            ans.push_back(total);
        }   else {
            while(cx <= cy && cx < 2) {
                total += layers[rx][cx++] ;
            }
            int l, r;
            if(cx <= cy) {
                {
                    int x = rx, y = cx;
                    int projDistance = min(x-2,y-2);
                    int px = x - projDistance, py = y - projDistance;
                    l = getLayerIndex(px, py);
                }
                {
                    int x = rx, y = cy;
                    int projDistance = min(x-2,y-2);
                    int px = x - projDistance, py = y - projDistance;
                    r = getLayer(px, py);
                }

                total += sums[2][r] - sums[2][l] + arrays[2][l];
            }
            ans.push_back(total);
        }
    }
    
    return ans;
}
