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
        for(int j=0;j<(int)cur_bits.size();j++)
            cur+=cur_bits[j][i]+'0';
        if(cur==known_pattern[i].substr(0, (int)cur_bits.size())) ret.push_back(i);
    }
    return ret;
}

bool get_bit(vector<bool> &msg, int ind)
{
    if((int)msg.size()<750)
    {
        if(ind==0) return 1;
        ind--;
        if(ind<10) return (((int)msg.size() >> ind) & 1);
        ind-=10;
        if(ind<(int)msg.size()) return msg[ind];
        return 0;
    }
    int x=(int)msg.size()%16;
    if(ind==0) return 0;
    ind--;
    if(ind<4) return ((x >> ind) & 1);
    ind-=4;
    if(ind<(int)msg.size()) return msg[ind];
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
        for(int j=1;j<(int)allies.size();j++) {
            cur[allies[j]] = get_bit(message, msg_pos++);
        }
        cur_bits.push_back(send_packet(cur));
        known = get_known_allies();
        if((int)known.size() == 1) break;
    }
    assert(known[0] == allies[0]);
    for(int i=0;i<(int)positions.size()-1;i++)
    {
        vector<bool> cur(BITS, 0);
        cur[known[0]] = positions[i];
        for(int j=1;j<(int)allies.size();j++) {
            cur[allies[j]] = get_bit(message, msg_pos++);
        }
        send_packet(cur);
    }
    while(msg_pos<(int)message.size()+((int)message.size()<750?11:5))
    {
        vector<bool> cur(BITS, 0);
        for(int j=0;j<(int)allies.size();j++) {
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
    for(int i=0;i<(int)received_bits.size();i++)
    {
        cur_bits.push_back(received_bits[i]);
        allies = get_known_allies();
        st++;
        if((int)allies.size() == 1) break;
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
    assert((int)allies.size()==16);
    vector<int> msg;
    for(int i=0;i<(int)received_bits.size();i++)
    {
        for(int j=(i<st?1:0);j<(int)allies.size();j++)
        {
            msg.push_back(received_bits[i][allies[j]]);
        }
    }
    int msglen = 0;
    for(int i=1;i<=(msg[0]?10:4);i++) msglen |= (msg[i]<<(i-1));
    vector<bool> ret;
    if(msg[0]==0)
    {
        int curm = (int)msg.size()-5;
        while(curm%16 != msglen) curm--;
        msglen = curm;
    }
    for(int i=(msg[0]?11:5);i<(msg[0]?11:5)+msglen;i++) ret.push_back(msg[i]);
    return ret;
}