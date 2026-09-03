#include "mosaic.h"
#include "bits/stdc++.h"
const int mxN = 1e6 + 5;
using ll = long long;
using namespace std;

vector<long long> layers[3];
vector<long long> sum[3];
map<int, int> mp[mxN];
int N, Q;
vector<ll> prefix, suffix;
void genLayers() {
  for(int i = 0; i < 3; i++) {
    if(i!=0) {
      for(int j = i; j < N; j++) {
        mp[i][j] = 1 - (mp[i-1][j] | mp[i][j-1]);
        mp[j][i] = 1 - (mp[j-1][i] | mp[j][i-1]);
      }
    }
    for(int j = N - 1; j >= i; --j) layers[i].push_back(mp[j][i]);
    for(int j = i + 1; j < N; j++) layers[i].push_back(mp[i][j]);
    ///got my layer
    ///generate the sum
    sum[i] = layers[i];
    for(int j = 1; j < layers[i].size(); j++) {
      sum[i][j] += sum[i][j-1];
    }
  }
  prefix = layers[2];
  suffix = layers[2];
  reverse(suffix.begin(), suffix.end());
  for(int i = 0; i < prefix.size(); i++) {
    prefix[i] *= i;
    if(i)prefix[i]+=prefix[i-1];
    suffix[i] *= i;
    if(i)suffix[i]+=suffix[i-1];
  }
}
int getLayerIndex(int x, int y) {
    int layer = min(x, y);
    if(y == layer) {
        return N - x - 1;
    }   else {
        return (N - layer - 1) + (y - layer);
    }

}
int getSum(int lay, int l, int r) {return sum[lay][r]-sum[lay][l]+layers[lay][l];} 
ll calcPrefixProjection(int l, int r) {
  return prefix[r] - (l == 0 ? 0 : prefix[l-1]) - 1ll * getSum(2, l, r) * (l - 1);
}
ll calcSuffixProjection(int l, int r) {
  int orgL = l, orgR = r;
  tie(l, r) = make_pair(layers[2].size()-r-1,layers[2].size()-l-1);
  return suffix[r] - (l == 0 ? 0 : suffix[l-1]) - 1ll * getSum(2, orgL, orgR) * (l - 1);  
}
std::vector<long long> mosaic(std::vector<int> X, std::vector<int> Y, std::vector<int> T, std::vector<int> B, std::vector<int> L, std::vector<int> R) {

    N = X.size();
    vector<int> layer(N * 2 - 1);
    for(int i = 0; i < N; i++) {
      mp[0][i] = X[i];
    }
    for(int i = 0; i < N; i++) {
      mp[i][0] = Y[i];
    }

    genLayers();

    vector<long long> ans;
    auto tmpB = B;
    for(int i = 0; i < T.size(); i++) {
        ///Rows A to C
        ///Columns B to D
        int A = T[i], C = tmpB[i];
        int B = L[i], D = R[i];
        long long total = 0;


        for (int i = 0; i < 2; ++i) {
            if (A == i && B == i) { ///both row and column
                total += getSum(i, N - 1 - C, (N-1-i) + (D-i));
                ++A, ++B;
            } else if (A == i) {  ///only row
                total += getSum(i, N - 1 - i + (B-i), N - 1 - i + (D-i));
                ++A;
            } else if (B == i) {  ///only column
                total += getSum(i, N - 1 - C, N - 1 - A);
                ++B;
            }
            if (A > C || B > D) break;
        }

        if (A <= C && B <= D) {
          ///The submatrix is divided into 3 parts
          ///1. increasing prefix from 1 to x
          ///2. decreasing suffix from 1 to x
          ///3. constant shit
          int l, r, offset;
          {
            int x = C, y = B;
            int projDistance = min(x-2,y-2);
            int px = x - projDistance, py = y - projDistance;
            int prefixL = getLayerIndex(px, py);
            int prefixR = prefixL + min(C-A,D-B)-1;
            if(prefixL <= prefixR) {
              total += calcPrefixProjection(prefixL, prefixR);
            }
            l = prefixR + 1;
            offset = (prefixR-prefixL+2);
          }
          {
            int x = A, y = D;
            int projDistance = min(x-2,y-2);
            int px = x - projDistance, py = y - projDistance;
            int prefixR = getLayerIndex(px, py);
            int prefixL = prefixR - min(C-A,D-B) + 1;
            if(prefixL <= prefixR) {
              total += calcSuffixProjection(prefixL, prefixR);
            }
            r = prefixL - 1;
          }
          if(l <= r) {
            total += 1ll*getSum(2, l, r) * offset;
          }
        }

        ans.push_back(total);
    }
    
    return ans;
}
