#include "mosaic.h"
#include "bits/stdc++.h"
using namespace std;

vector<long long> mosaic(vector<int> X, vector<int> Y, vector<int> T, vector<int> B, vector<int> L, vector<int> R) {
    vector<long long> ans;
    for(int i = 0; i < T.size(); i++) {
        int rx = T[i], ry = B[i];
        int cx = L[i], cy = R[i];
        rx = max(rx, 1);
        cx = max(cx, 1);
        if(rx <= ry && cx <= cy) {
            long long area = 1ll*(ry-rx+1)*(cy-cx+1);
            ans.push_back((area+((rx+cx)&1^1)) / 2);
        }   else ans.push_back(0);
    }
    
    return ans;
}
