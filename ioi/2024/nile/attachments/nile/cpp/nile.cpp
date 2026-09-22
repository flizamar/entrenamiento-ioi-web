#include "nile.h"
#include<algorithm>
using namespace std;

vector<long long> calculate_costs(vector<int> W, vector<int> A, vector<int> B, vector<int> E) {
  int Q = (int)E.size();
  vector<long long> R(Q, 0);
  int N = (int)W.size();
  vector<int> difs(N, 0);
  long long totsum = 0;
  for(int i = 0; i < N; i++){
    difs[i] = A[i] - B[i];
    totsum += B[i];
  }
  sort(difs.begin(), difs.end());
  for(int i = 0; i < Q; i++){
    R[i] = totsum;
    if(N % 2){
      if(E[i] == 1){
        int min_diff = 1e9+5;
        for(int j = 0; j < N; j+=2){
          if(A[j] - B[j] < min_diff) min_diff = A[j] - B[j];
        }
        R[i] += min_diff;
      }
      else{
        R[i] += difs[0];
      }
    }
  }
  return R;
}
