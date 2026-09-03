#include "bits/stdc++.h"
#include "message.h"

using namespace std;

using Message = vector<bool>;
using Packet = vector<bool>;

const int BITS = 31;

void send_message(Message M, vector<bool> C) {
  for (int i = 0; i < (int)M.size(); i += 2) {
    Packet p(BITS);
    int mine = 0;
    for (int b = 0; b < BITS; b++) {
      if (!C[b]) {
        p[b] = M[i + (mine / 8)];
        mine++;
      }
    }
    assert(mine == 16);
    send_packet(p);
  }
}

Message receive_message(vector<Packet> R) {
  Message result;
  for (int i = 0; i < (int)R.size(); i++) {
    vector<bool> repeated;
    for (int b = 0; b + 1 < BITS; b++) {
      if (R[i][b] == R[i][b + 1])
        repeated.push_back(R[i][b]);
    }
    result.push_back(repeated.front());
    result.push_back(repeated.back());
  }
  return result;
}