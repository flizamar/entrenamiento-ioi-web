#include "message.h"
#include <bits/stdc++.h>

using namespace std;

/*
 * The solution start by identifing one on allies in 4 rounds
 * There will be 16+8+4+2 = 30 wasted bits and 16*4-30 = 34 bits that can be used to transmit the message
 * Use 5 of them to indicate the number of bits you need to remove from the message. (29 bits remaining)
 * in 29 rounds you can send for each bit whether it's ally or spy and use the remaining bits for the message
 * You can skip the ally you already know and the last bit is already known so you need exactly 29 more rounds.
 * If the message will take partial bits from 29 rounds we send [11111] [len of actual message], 
 * otherwise 5 bits are enough to indicate number of needed bits to be removed.
*/

const int BITS=31;

void send_message(std::vector<bool> message, std::vector<bool> positions) {
    vector<int> cur_subset(BITS), cur_allies, free_allies;
    iota(cur_subset.begin(), cur_subset.end(), 0);
    for(int i=0;i<BITS;i++)
        if(positions[i]==0) cur_allies.push_back(i);
    int free_bits=(16*4-(16+8+4+2 + 5));
    vector<bool> sent_bits;
    if ((int)message.size() <= free_bits - 9 + 29*15) {
        for(int i=0;i<5;i++) sent_bits.push_back(1);
        int msg_len = message.size();
        for(int i=0;i<9;i++) {
            sent_bits.push_back(msg_len%2);
            msg_len/=2;
        }
        for(int i=0;i<(int)message.size();i++) sent_bits.push_back(message[i]);
        while((int)sent_bits.size() < free_bits + 29*15) sent_bits.push_back(0);
    } else {
        int rounds = ((int)message.size() + 5 + 15) / 16;
        int remove_bits = rounds*16 - (message.size()), tmp_rem=remove_bits;
        for(int i=0;i<5;i++) {
            sent_bits.push_back(tmp_rem%2);
            tmp_rem/=2;
        }
        for(int i=0;i<(int)message.size();i++) sent_bits.push_back(message[i]);
        while(sent_bits.size() % 16) {sent_bits.push_back(0);}
    }
    int cur_pos = 0;
    for(int _=0;_<4;_++)
    {
        vector<bool> msg(BITS);
        for(int x=0;x<(int)cur_allies.size()/2;x++) msg[cur_allies[x]]=0;
        for(int x=cur_allies.size()/2;x<(int)cur_allies.size();x++) msg[cur_allies[x]]=1;
        sort(free_allies.begin(), free_allies.end());
        for(int i:free_allies) 
            msg[i] = sent_bits[cur_pos++];
        msg=send_packet(msg);
        int co[2]={};
        for(int i:cur_subset) co[msg[i]]++;
        int used_bit = co[0]<co[1]? 0 : 1;
        vector<int> new_subset, new_allies;
        for(int i:cur_subset) {
            if(msg[i] == used_bit) new_subset.push_back(i);
        }
        for(int i:cur_allies) {
            if(msg[i] == used_bit) new_allies.push_back(i);
            else free_allies.push_back(i);
        }
        cur_subset = new_subset;
        cur_allies = new_allies;
    }
    
    assert(cur_allies.size() == 1);
    int known_ally = cur_allies[0];
    vector<bool> allies_positions;
    for(int i=0;i<BITS;i++)
    {
        if(i==known_ally) continue;
        allies_positions.push_back(positions[i]);
    }
    allies_positions.pop_back();
    for(int i=0;i<29;i++) {
        vector<bool> msg(BITS);
        for(int x=0;x<BITS;x++)
        {
            if(x==known_ally) msg[x]=allies_positions[i];
            else if(positions[x]==0) 
                msg[x] = sent_bits[cur_pos++];
        }
        send_packet(msg);
    }
    while(cur_pos < (int)sent_bits.size()) {
        vector<bool> msg(BITS);
        for(int i=0;i<BITS;i++) {
            if(positions[i]==0)
                msg[i]=sent_bits[cur_pos++];
        }
        send_packet(msg);
    }
}

std::vector<bool> receive_message(vector<vector<bool>> received_bits) {
    vector<int> cur_subset(BITS);
    iota(cur_subset.begin(),cur_subset.end(), 0);
    for(int r=0;r<4;r++) {
        int co[2]={};
        for(int i:cur_subset) co[received_bits[r][i]]++;
        int used_bit = co[0]<co[1]? 0 : 1;
        vector<int> new_subset;
        for(int i:cur_subset)
            if(received_bits[r][i] == used_bit) new_subset.push_back(i);
        cur_subset = new_subset;
    }
    assert(cur_subset.size() == 1);
    int known_ally = cur_subset[0];
    vector<bool> positions;
    int cur_ally=0, curtotal=15;
    for(int i=4;i<4+29;i++) {
        if(cur_ally == known_ally) {
            positions.push_back(0); cur_ally++;
        }
        positions.push_back(received_bits[i][known_ally]);
        cur_ally++; curtotal -= positions.back();
    }
    if(cur_ally==known_ally) positions.push_back(0); // Position before last is known ally
    positions.push_back(curtotal);
    if(positions.size()<31) positions.push_back(0); // Last position is known ally
    vector<int> ally_positions;
    for(int i=0;i<BITS;i++)
        if(positions[i]==0) ally_positions.push_back(i);
    vector<bool> msg;
    cur_subset = vector<int>(BITS);
    iota(cur_subset.begin(),cur_subset.end(), 0);
    for(int r=0;r<4;r++) {
        int bad[BITS]={};
        int co[2]={};
        for(int i:cur_subset) {bad[i] = 1; co[received_bits[r][i]]++;}
        int used_bit = co[0]<co[1]? 0 : 1;
        vector<int> new_subset;
        for(int i:cur_subset) {
            if(received_bits[r][i] == used_bit) {
                new_subset.push_back(i);
            }
        }
        cur_subset = new_subset;
        for(int i:ally_positions)
            if(!bad[i]) msg.push_back(received_bits[r][i]);
    }
    for(int i=4;i<4+29;i++) {
        for(int j:ally_positions) {
            if(j==known_ally) continue;
            msg.push_back(received_bits[i][j]);
        }
    }
    for(int i=4+29;i<(int)received_bits.size();i++) {
        for(int j:ally_positions) msg.push_back(received_bits[i][j]);
    }
    int case1=1;
    for(int i=0;i<5;i++) case1&=msg[i];
    if(case1) {
        int len=0;
        for(int i=9+5-1;i>=5;i--) {
            len*=2; len+=msg[i];
        }
        vector<bool> res;
        for(int i=9+5;i<9+5+len;i++)
            res.push_back(msg[i]);
        return res;
    } else {
        vector<bool> cur_msg;
        for(int i=5;i<(int)msg.size();i++) cur_msg.push_back(msg[i]);
        int rem=0;
        for(int i=4;i>=0;i--) {
            rem*=2; rem+=msg[i];
        }
        while(rem--) cur_msg.pop_back();
        return cur_msg;
    }
}