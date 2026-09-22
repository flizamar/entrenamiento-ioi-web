#include "message.h"
#include <bits/stdc++.h>

using namespace std;

const int BITS=31;

void send_message(std::vector<bool> message, std::vector<bool> positions) {
    int ind=-1;
    for(int i=0;i<BITS;i++) {
        if(positions[i] == 0 && positions[i] == positions[(i+1)%BITS]) {
            ind=i; break;
        }
    }
    for(int i=0;i<5;i++)
    {
        send_packet(vector<bool>(BITS, (ind&(1<<i))?1:0));
    }
    for(int i=0;i<positions.size()/2;i++)
    {
        vector<bool> msg(BITS,0);
        msg[ind]=positions[i*2];
        msg[(ind+1)%BITS]=positions[i*2+1];
        send_packet(msg);
    }
    int rounds = (message.size()+10+15)/16;
    int rem = (rounds*16) - (message.size()+10);
    vector<bool> sent_bits;
    for(int i=0;i<10;i++)
        sent_bits.push_back((message.size()&(1<<i))?1:0);
    for(int i=0;i<message.size();i++) sent_bits.push_back(message[i]);
    for(int i=0;i<rem;i++) sent_bits.push_back(0);
    vector<int> allies;
    for(int i=0;i<BITS;i++) if(positions[i] == 0) allies.push_back(i);
    for(int i=0;i<(int)sent_bits.size();i+=16) {
        vector<bool> msg(BITS);
        for(int j=0;j<16;j++) {
            msg[allies[j]] = sent_bits[i + j];
        }
        send_packet(msg);
    }
    return;
}

std::vector<bool> receive_message(std::vector<std::vector<bool>> received_bits) {
    int ind=0;
    for(int i=0;i<5;i++) {
        sort(received_bits[i].begin(), received_bits[i].end());
        if(received_bits[i][15]) ind|=(1<<i);
    }
    int co=15;
    vector<int> positions(BITS);
    for(int i=5;i<5+15;i++)
    {
        int cind = (i-5)*2;
        positions[cind] = received_bits[i][ind];
        positions[cind+1] = received_bits[i][(ind+1)%BITS];
        co-=positions[cind]; co-=positions[cind+1];
    }
    positions.back() = co;
    vector<int> allies;
    for(int i=0;i<BITS;i++) {
        if(positions[i] == 0) allies.push_back(i);
    }
    vector<bool> sent_bits;
    for(int i=5+15;i<received_bits.size();i++) {
        for(int j:allies)
            sent_bits.push_back(received_bits[i][j]);
    }
    int len=0;
    for(int i=0;i<10;i++) if(sent_bits[i]) len|=(1<<i);
    vector<bool> res;
    for(int i=10;i<sent_bits.size();i++) res.push_back(sent_bits[i]);
    int rem=res.size()-len;
    while(rem--) res.pop_back();
    return res;
}