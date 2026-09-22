#include "message.h"

#include <bits/stdc++.h>

using namespace std;

const int BITS=31;

int get(vector<bool> &v, int ind)
{
  if(ind<v.size()) return v[ind];
  return 0;
}

void send_message(vector<bool> message, vector<bool> positions) {
    int ind = -1;
    for(int i=0;i<positions.size();i++)
        if(positions[i]==0) {ind=i; break;}
    for(int i=0;i<4;i++) {
        vector<bool> cur(BITS, (ind&(1<<i))?1:0);
        send_packet(cur);
    }
    vector<bool> msg;
    // 5 (len) + msg.size() + 5*16 + 30 = 70 round
    int len = 0, free_bits = 30 * 15, fl;
    if(message.size() < free_bits) {
      fl=1;
      len = message.size();
    } else {
      // 5(len) msg .... 0000
      fl=0;
      len = (16-(message.size() + 5 - free_bits)%16)%16;
    }
    msg.push_back(fl);
    for(int i=0;i<(fl?10:4);i++)
      msg.push_back((len&(1<<i))?1:0);
    for(int i:message) msg.push_back(i);
    int msg_ind=0;
    for(int i=0;i<positions.size()-1;i++) {
      vector<bool> cur(BITS);
      for(int j=0;j<BITS;j++) {
        if(j == ind) {
          cur[j] = positions[i];
        }
        else if(positions[j] == 0) {
          cur[j] = get(msg, msg_ind++);
        }
      }
      send_packet(cur);
    }
    while(msg_ind < msg.size())
    {
      vector<bool> cur(BITS);
      for(int j=0;j<BITS;j++) {
        if(positions[j]==1) continue;
        cur[j] = get(msg, msg_ind++);
      }
      send_packet(cur);
    }
}

vector<bool> receive_message(vector<vector<bool>> received_bits) {
  int ind=0;
  for(int i=0;i<4;i++) {
    sort(received_bits[i].begin(), received_bits[i].end());
    ind |= (received_bits[i][15]<<i);
  }
  int rem=15;
  vector<int> positions;
  for(int i=4;i<4+30;i++) {
    positions.push_back(received_bits[i][ind]);
    rem-=positions.back();
  }
  positions.push_back(rem);
  vector<int> rcv;
  for(int i=4;i<4+30;i++) {
    for(int j=0;j<BITS;j++) {
      if(positions[j] == 0 && j != ind)
        rcv.push_back(received_bits[i][j]);
    }
  }
  for(int i=4+30;i<received_bits.size();i++) {
    for(int j=0;j<BITS;j++) {
      if(positions[j]==0) rcv.push_back(received_bits[i][j]);
    }
  }
  if(rcv[0] == 0) {
    int rem=0;
    for(int i=1;i<5;i++) rem|=(rcv[i]<<(i-1));
    while(rem--) rcv.pop_back();
    vector<bool> ret;
    for(int i=5;i<rcv.size();i++) ret.push_back(rcv[i]);
    return ret;
  } else {
    int len=0;
    for(int i=1;i<11;i++) len|=(rcv[i]<<(i-1));
    vector<bool> ret;
    for(int i=11;i<rcv.size();i++) ret.push_back(rcv[i]);
    while(ret.size() > len) ret.pop_back();
    return ret;
  }
}