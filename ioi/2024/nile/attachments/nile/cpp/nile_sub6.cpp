#include "nile.h"
typedef long long ll;

using namespace std;
vector<long long> calculate_costs(vector<int> w, vector<int> a,
                                       vector<int> b, vector<int> e) {

  int q = (int)e.size();

  vector<long long> r(q, 0);
  vector<ll> c(w.size());
  ll sumb  = 0;
  for(int i = 0; i < w.size(); i++){
    c[i] = a[i]-b[i];
    sumb += b[i];
  }
  ll total;
  for(int _ = 0; _ < q; _++){
    total = sumb;

    for(int i = 0; i < w.size()-1; i++){
      if(w[i+1]-w[i]<=e[_]){
        if(i == w.size()-3) total++;
        i++;
      }else{
        total++;
        if(i == w.size()-2) total++;
      }
    }
    r[_]=total;
  }

  return r;
}