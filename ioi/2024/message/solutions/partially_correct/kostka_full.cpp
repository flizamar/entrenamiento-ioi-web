#include "bits/stdc++.h"
#include "message.h"

using namespace std;
using Message = vector<bool>;
using Packet = vector<bool>;

const int BITS = 31;

Packet my_send_packet(Packet P)
{
    // for (auto p : P)
    //     cerr << p;
    // cerr << "\n";
    return send_packet(P);
}

const int TRESHOLD = 16 * 30;

void send_message(Message M, vector<bool> C)
{
    int margin = 4;
    bool special = false;
    if (M.size() <= TRESHOLD) {
        margin += 10;
        special = true;
    }
    int rest = M.size() % (1<<margin);
    for (int b = 1; b < (1<<margin); b *= 2)
    {
        if (rest & b) {
            M.push_back(1);
        }
        else {
            M.push_back(0);
        }
    }
    M.push_back(special);
    vector<int> allies;
    for (int i = 0; i < BITS; i++)
    {
        if (!C[i])
            allies.push_back(i);
    }
    auto current_allies = allies;
    vector<int> potential_allies, not_needed_allies;
    for (int i = 0; i < BITS; i++)
    {
        potential_allies.push_back(i);
    }
    for (int ph = 0; ph < 4; ph++)
    {
        Packet packet(BITS);
        for (size_t i = 0; i < current_allies.size() / 2; i++)
        {
            packet[current_allies[i]] = 0;
        }
        for (size_t i = current_allies.size() / 2; i < current_allies.size(); i++)
        {
            packet[current_allies[i]] = 1;
        }
        sort(not_needed_allies.begin(), not_needed_allies.end());
        for (int ally : not_needed_allies)
        {
            if (!M.empty())
            {
                packet[ally] = M.back();
                M.pop_back();
            }
        }
        Packet received_packet = my_send_packet(packet);
        int zeroes_count = 0, ones_count = 0;
        for (auto ally : potential_allies)
        {
            if (received_packet[ally])
                ones_count++;
            else
                zeroes_count++;
        }
        bool smaller_count = (ones_count > zeroes_count ? 0 : 1);
        vector<int> new_allies;
        for (auto ally : current_allies)
        {
            if (received_packet[ally] == smaller_count)
            {
                new_allies.push_back(ally);
            }
            else
            {
                not_needed_allies.push_back(ally);
            }
        }
        current_allies = new_allies;
        vector<int> new_potential_allies;
        for (auto ally : potential_allies)
        {
            if (received_packet[ally] == smaller_count)
            {
                new_potential_allies.push_back(ally);
            }
        }
        potential_allies = new_potential_allies;
    }
    assert(current_allies.size() == 1);
    int selected_ally = current_allies[0];
    vector<bool> to_be_sent_by_selected_ally;
    for (int i = 0; i + (selected_ally == 30 ? 2 : 1) < BITS; i++)
    {
        if (i == selected_ally)
            continue;
        to_be_sent_by_selected_ally.push_back(C[i]);
    }
    reverse(to_be_sent_by_selected_ally.begin(), to_be_sent_by_selected_ally.end());
    while (!M.empty() or !to_be_sent_by_selected_ally.empty())
    {
        vector<bool> packet(BITS);
        for (auto ally : allies)
        {
            if (ally == selected_ally && !to_be_sent_by_selected_ally.empty())
            {
                packet[ally] = to_be_sent_by_selected_ally.back();
                to_be_sent_by_selected_ally.pop_back();
            }
            else if (!M.empty())
            {
                packet[ally] = M.back();
                M.pop_back();
            }
        }
        my_send_packet(packet);
    }
}

Message receive_message(vector<Packet> R)
{
    vector<int> potential_allies;
    for (int i = 0; i < BITS; i++)
    {
        potential_allies.push_back(i);
    }
    vector <bool> smallers;
    for (int ph = 0; ph < 4; ph++)
    {
        int zeroes_count = 0, ones_count = 0;
        for (auto ally : potential_allies)
        {
            if (R[ph][ally])
                ones_count++;
            else
                zeroes_count++;
        }
        bool smaller_count = (ones_count > zeroes_count ? 0 : 1);
        smallers.push_back(smaller_count);
        vector<int> new_potential_allies;
        for (auto ally : potential_allies)
        {
            if (R[ph][ally] == smaller_count)
            {
                new_potential_allies.push_back(ally);
            }
        }
        potential_allies = new_potential_allies;
    }
    assert(potential_allies.size() == 1);
    int selected_ally = potential_allies[0];
    vector<int> my_C;
    for (int ph = 4; ph < 33; ph++)
    {
        my_C.push_back(R[ph][selected_ally]);
    }
    int sum_C = 0;
    for (int i = 0; i < (int)my_C.size(); i++)
    {
        sum_C += my_C[i] ? 1 : 0;
    }
    my_C.push_back(15 - sum_C);
    my_C.insert(my_C.begin() + selected_ally, 0);
    assert(my_C.size() == 31);
    set<int> available_allies;
    Message N;
    for (size_t ph = 0; ph < R.size(); ph++)
    {
        if (ph == 33)
        {
            available_allies.insert(selected_ally);
        }
        for (auto ally : available_allies)
        {
            N.push_back(R[ph][ally]);
        }
        if (ph < 4)
        {
            for (int i=0; i<BITS; i++) {
                if (!my_C[i] and i != selected_ally) {
                    if (R[ph][i] != smallers[ph]) {
                        available_allies.insert(i);
                    }
                }
            }
        }
    }
    int margin = 4;
    if (N[0]) margin += 10;
    int rest = 0;
    for (int i=0; i<margin; i++) {
        if (N[margin-i]) rest |= (1<<i);
    }
    vector<bool> message(N.begin()+margin+1, N.end());
    while ((int)message.size() % (1<<margin) != rest) message.pop_back();
    reverse(message.begin(), message.end());
    return message;
}
