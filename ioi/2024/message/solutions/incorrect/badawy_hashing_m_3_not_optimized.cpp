#include "message.h"

#include <bits/stdc++.h>

using namespace std;

const int BITS=31;

string known_pattern[BITS];
vector<vector<bool>> cur_bits;

vector<int> get_known_allies()
{
    vector<int> ret;
    for(int i=0;i<BITS;i++)
    {
        string cur="";
        for(int j=0;j<cur_bits.size();j++)
            cur+=cur_bits[j][i]+'0';
        if(cur==known_pattern[i].substr(0, cur_bits.size())) ret.push_back(i);
    }
    return ret;
}

bool get_bit(vector<bool> &msg, int ind)
{
    if(ind<10) return ((msg.size() >> ind) & 1);
    ind-=10;
    if(ind<msg.size()) return msg[ind];
    return 0;
}

void send_message(vector<bool> message, vector<bool> positions) {
    srand(123456);
    for(int i=0;i<BITS;i++)
    {
        known_pattern[i]="";
        for(int j=0;j<100;j++)
            known_pattern[i] += '0'+rand()%2;
    }
    cur_bits.clear();
    vector<int> allies;
    for(int i=0;i<BITS;i++) if(positions[i]==0) allies.push_back(i);
    vector<int> known;
    int msg_pos=0;
    for(int i=0;i<100;i++)
    {
        vector<bool> cur(BITS, 0);
        cur[allies[0]] = known_pattern[allies[0]][i] - '0';
        for(int j=1;j<allies.size();j++) {
            cur[allies[j]] = get_bit(message, msg_pos++);
        }
        cur_bits.push_back(send_packet(cur));
        known = get_known_allies();
        if(known.size() == 1) break;
    }
    assert(known[0] == allies[0]);
    for(int i=0;i<positions.size()-1;i++)
    {
        vector<bool> cur(BITS, 0);
        cur[known[0]] = positions[i];
        for(int j=1;j<allies.size();j++) {
            cur[allies[j]] = get_bit(message, msg_pos++);
        }
        send_packet(cur);
    }
    while(msg_pos<message.size()+10)
    {
        vector<bool> cur(BITS, 0);
        for(int j=0;j<allies.size();j++) {
            cur[allies[j]] = get_bit(message, msg_pos++);
        }
        send_packet(cur);
    }
}

vector<bool> receive_message(vector<vector<bool>> received_bits) {
    srand(123456);
    cur_bits.clear();
    for(int i=0;i<BITS;i++)
    {
        known_pattern[i]="";
        for(int j=0;j<100;j++)
            known_pattern[i] += '0'+rand()%2;
    }
    int st=0;
    vector<int> allies;
    for(int i=0;i<received_bits.size();i++)
    {
        cur_bits.push_back(received_bits[i]);
        allies = get_known_allies();
        st++;
        if(allies.size() == 1) break;
    }
    vector<bool> pos;
    int curc=15;
    for(int i=0;i<BITS-1;i++)
    {
        pos.push_back(received_bits[st][allies[0]]);
        curc-=pos.back();
        st++;
    }
    pos.push_back(curc);
    allies.clear();
    for(int i=0;i<BITS;i++) if(pos[i]==0) allies.push_back(i);
    assert(allies.size()==16);
    vector<int> msg;
    for(int i=0;i<received_bits.size();i++)
    {
        for(int j=(i<st?1:0);j<allies.size();j++)
        {
            msg.push_back(received_bits[i][allies[j]]);
        }
    }
    int msglen = 0;
    for(int i=0;i<10;i++) msglen |= (msg[i]<<i);
    vector<bool> ret;
    assert(10+msglen <= msg.size());
    for(int i=10;i<10+msglen;i++) ret.push_back(msg[i]);
    return ret;
}