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
    for(int i=0;i<100;i++)
    {
        vector<bool> cur(BITS, 0);
        for(int j=0;j<allies.size();j++) cur[allies[j]]=known_pattern[allies[j]][i]-'0';
        cur_bits.push_back(send_packet(cur));
        known = get_known_allies();
        if(known.size() == 16) break;
    }
    vector<int> msg;
    int rem=(16 - (4+message.size())%16)%16;
    for(int i=0;i<4;i++) msg.push_back((rem&(1<<i)) > 0);
    for(int i:message) msg.push_back(i);
    int msg_pos = 0;
    while(msg_pos < msg.size())
    {
        vector<bool> cur(BITS, 0);
        for(int i:allies) {
            cur[i]=(msg_pos < msg.size()? msg[msg_pos] : 0);
            msg_pos++;
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
        if(allies.size() == 16) break;
    }
    vector<int> msg;
    while(st<received_bits.size())
    {
        for(int i:allies) msg.push_back(received_bits[st][i]);
        st++;
    }
    int rem=0;
    for(int i=0;i<4;i++) if(msg[i]) rem|=(1<<i);
    while(rem--) msg.pop_back();
    vector<bool> ret;
    for(int i=4;i<msg.size();i++) ret.push_back(msg[i]);
    return ret;
}