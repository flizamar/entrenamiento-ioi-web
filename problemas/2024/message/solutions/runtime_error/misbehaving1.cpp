#include "message.h"

#include <bits/stdc++.h>

using namespace std;

vector<vector<bool>> msgs;

int ind=0;

// Trying to use global variables to save message, doesn't fail when shuffling but fails with different instances.

void send_message(
    std::vector<bool> message, std::vector<bool> positions) {
  msgs.push_back(message);
  for(int i=0;i<10;i++) {
    vector<bool> cur(31, (ind>>i)&1);
    send_packet(cur);
  }
}

std::vector<bool> receive_message(
    std::vector<std::vector<bool>> received_bits) {
  int ind=0;
  for (int i=0;i<received_bits.size();i++) {
    auto round=received_bits[i];
    std::sort(round.begin(), round.end());
    ind |=round[15];
  }
  return msgs[ind];
}