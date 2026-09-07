#include "message.h"
#include <bits/stdc++.h>

using namespace std;

const int BITS=31;

void send_message(std::vector<bool> message, std::vector<bool> positions) {
    int ind=-1;
    for(int i=0;i+1<BITS;i++) {
        if(positions[i] == 0 && positions[i] == positions[i+1]) {
            ind=i; break;
        }
    }
    for(int i=0;i<5;i++)
    {
        send_packet(vector<bool>(BITS, (ind&(1<<i))?1:0));
    }
    vector<int> npos;
    for(int i=0;i<positions.size();i++) {
        if(i==ind||i==ind+1) continue;
        npos.push_back(positions[i]);
    }
    npos.pop_back();
    for(int i=0;i<npos.size()/2;i++)
    {
        vector<bool> msg(BITS,0);
        msg[ind]=npos[i*2];
        msg[ind+1]=npos[i*2+1];
        send_packet(msg);
    }
    int rounds = (message.size()+5+15)/16;
    int rem = (rounds*16) - (message.size()+5);
    vector<bool> sent_bits;
    for(int i=0;i<5;i++)
        sent_bits.push_back((rem&(1<<i))?1:0);
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

void add_pos(vector<bool> &pos, int v, int ind) {
    if(pos.size()==ind) {
        pos.push_back(0); 
        pos.push_back(0); 
    }
    pos.push_back(v);
    if(pos.size()==ind) {
        pos.push_back(0); 
        pos.push_back(0); 
    }
}

std::vector<bool> receive_message(std::vector<std::vector<bool>> received_bits) {
    int ind=0;
    for(int i=0;i<5;i++) {
        sort(received_bits[i].begin(), received_bits[i].end());
        if(received_bits[i][15]) ind|=(1<<i);
    }
    int co=15;
    vector<bool> positions;
    for(int i=5;i<5+14;i++)
    {
        add_pos(positions, received_bits[i][ind], ind);
        co-=received_bits[i][ind];
        add_pos(positions, received_bits[i][ind+1], ind);
        co-=received_bits[i][ind+1];
    }
    add_pos(positions, co, ind);
    vector<int> allies;
    for(int i=0;i<BITS;i++) {
        if(positions[i] == 0) allies.push_back(i);
    }
    vector<bool> sent_bits;
    for(int i=5+14;i<received_bits.size();i++) {
        for(int j:allies)
            sent_bits.push_back(received_bits[i][j]);
    }
    int rem=0;
    for(int i=0;i<5;i++) if(sent_bits[i]) rem|=(1<<i);
    vector<bool> res;
    for(int i=5;i<sent_bits.size();i++) res.push_back(sent_bits[i]);
    while(rem--) res.pop_back();
    return res;
}