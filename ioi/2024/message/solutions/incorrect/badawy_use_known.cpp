#include "message.h"

#include <iostream>
#include <vector>
#include <algorithm>

const int LG=17, POS=31;
using namespace std;

void send(int &ind, vector<int> &kn,vector<bool> sent) {
    if(kn.size() == 0) {
        vector<bool> cur(POS, sent[ind]);
        if(sent[ind] == 0) kn.push_back(ind);
        ind++;
        send_packet(cur);
    } else {
        vector<int> tba;
        vector<bool> cur(POS);
        for(int i=0;i<kn.size();i++) {
            if(ind+i < POS && sent[ind+i] == 0) {
                tba.push_back(ind+i);
            }
            cur[kn[i]] = (ind+i < sent.size() ? sent[ind+i] : 0);
        }
        ind+=kn.size();
        send_packet(cur);
        for(int i:tba)
            kn.push_back(i);
    }
}

void send_message(std::vector<bool> message, std::vector<bool> positions) {
    vector<bool> sent;
    for(int i=0;i<positions.size();i++)
        sent.push_back(positions[i]);
    int len=message.size();
    for(int i=0;i<10;i++) sent.push_back((len>>i)&1);
    for(int i=0;i<message.size();i++)  sent.push_back(message[i]);
    vector<int> kn;
    int ind=0;
    while(ind<sent.size()) {
        send(ind, kn, sent);
    }
}

void receive(vector<int> &kn, vector<bool> &msg, vector<bool> received) {
    if(kn.size() == 0) {
        sort(received.begin(), received.end());
        msg.push_back(received[15]);
        if(msg.back() == 0) {
            kn.push_back(msg.size() - 1);
        }
    } else {
        vector<int> tba;
        for(int i=0;i<kn.size();i++) {
            msg.push_back(received[kn[i]]);
            if(msg.back()==0 && msg.size() <= POS) {
                tba.push_back(msg.size() - 1);
            }
        }
        for(int i:tba) kn.push_back(i);
    }
}

std::vector<bool> receive_message(std::vector<std::vector<bool>> received_bits) {
    vector<bool> msg;
    vector<int> kn;
    for(int i=0;i<received_bits.size();i++) receive(kn, msg, received_bits[i]);
    int len=0;
    for(int i=POS;i<POS+10;i++) len|=msg[i]<<(i-POS);
    vector<bool> rel;
    for(int i=POS+10;i<msg.size();i++) rel.push_back(msg[i]);
    while(rel.size() > len) rel.pop_back();
    return rel;
}