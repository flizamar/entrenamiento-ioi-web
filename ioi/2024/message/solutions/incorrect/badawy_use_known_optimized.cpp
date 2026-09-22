#include "message.h"

#include <iostream>
#include <vector>
#include <algorithm>

const int LG=17, POS=31;
using namespace std;

vector<vector<bool> > sent_items;

void send(int &ind, vector<int> &kn,vector<bool> sent, bool rev) {
    if(kn.size() == 0) {
        vector<bool> cur(POS, sent[ind]);
        if(sent[ind] == 0) kn.push_back((rev? POS-ind-1:ind));
        ind++;
        sent_items.push_back(cur);
    } else {
        vector<int> tba;
        vector<bool> cur(POS);
        for(int i=0;i<kn.size();i++) {
            if(ind+i < POS && sent[ind+i] == 0) {
                tba.push_back((rev? POS-(ind+i) -1 : ind+i));
            }
            cur[kn[i]] = (ind+i < sent.size() ? sent[ind+i] : 0);
        }
        ind+=kn.size();
        sent_items.push_back(cur);
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
    int ind=0; sent_items.clear();
    while(ind<sent.size()) {
        send(ind, kn, sent, 0);
    }
    reverse(sent.begin(), sent.begin()+positions.size());
    vector<vector<bool> > cur=sent_items;
    ind=0; kn.clear();
    sent_items.clear();
    while(ind<sent.size()) {
        send(ind, kn, sent, 1);
    }
    if(cur.size()<sent_items.size()) {
        send_packet(vector<bool>(POS, 0));
        for(auto i:cur) send_packet(i);
    } else {
        send_packet(vector<bool>(POS, 1));
        for(auto i:sent_items) send_packet(i);
    }
    cerr << cur.size() << " " << sent_items.size() << endl;
}

void receive(vector<int> &kn, vector<bool> &msg, vector<bool> received, bool rev) {
    if(kn.size() == 0) {
        sort(received.begin(), received.end());
        msg.push_back(received[15]);
        if(msg.back() == 0) {
            kn.push_back(rev? POS- (msg.size() - 1) - 1 : msg.size()-1);
        }
    } else {
        vector<int> tba;
        for(int i=0;i<kn.size();i++) {
            msg.push_back(received[kn[i]]);
            if(msg.back()==0 && msg.size() <= POS) {
                tba.push_back(rev? POS- (msg.size() - 1) - 1 : msg.size()-1);
            }
        }
        for(int i:tba) kn.push_back(i);
    }
}

std::vector<bool> receive_message(std::vector<std::vector<bool>> received_bits) {
    vector<bool> msg;
    vector<int> kn;
    sort(received_bits[0].begin(), received_bits[0].end());
    bool rev=received_bits[0][15];
    for(int i=1;i<received_bits.size();i++) receive(kn, msg, received_bits[i], rev);
    int len=0;
    for(int i=POS;i<POS+10;i++) len|=msg[i]<<(i-POS);
    vector<bool> rel;
    for(int i=POS+10;i<msg.size();i++) rel.push_back(msg[i]);
    while(rel.size() > len) rel.pop_back();
    return rel;
}