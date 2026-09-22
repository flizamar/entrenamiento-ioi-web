#include "message.h"

#include <iostream>
#include <vector>
#include <algorithm>

const int LG=17, POS=31;
using namespace std;

void send_message(std::vector<bool> message, std::vector<bool> positions) {
    for (int i=0;i<POS;i++) {
        std::vector<bool> bits(POS);
        for (int j=0; j<POS; j++) bits[j] = positions[i];
        send_packet(bits);
    }
    std::vector<int> needed, mine;
    for(int i=0;i<POS;i++)
        if(positions[i]==0) mine.push_back(i);
    int sz=message.size();
    for(int i=0;i<LG;i++)
    {
        needed.push_back(sz%2);
        sz/=2;
    }
    for(int i=0;i<message.size();i++) needed.push_back(message[i]);
    int rem=16-(needed.size()%16);
    rem%=16;
    for(int i=0;i<rem;i++) needed.push_back(0);
    for(int round=0;round<needed.size()/16;round++)
    {
        std::vector<bool> bits(POS);
        for(int j=0;j<16;j++)
            bits[mine[j]]=needed[round*16+j];
        send_packet(bits);
    }
}

std::vector<bool> receive_message(std::vector<std::vector<bool>> received_bits) {
    std::vector<bool> message;
    std::vector<int> mine;
    for(int i=0;i<31;i++)
    {
        sort(received_bits[i].begin(), received_bits[i].end());
        if(received_bits[i][15]==0) mine.push_back(i);
    }
    std::vector<bool> received;
    for(int i=31;i<received_bits.size();i++)
    {
        for(int j:mine) received.push_back(received_bits[i][j]);
    }
    int len=0;
    for(int i=LG-1;i>=0;i--)
    {
        len*=2; len+=received[i];
    }
    for(int i=LG;i<LG+len;i++)
        message.push_back(received[i]);
    return message;
}