#include "message.h"

#include <iostream>
#include <vector>
#include <algorithm>

void send_message(
    std::vector<bool> message, std::vector<bool> /*positions*/) {
  for (auto bit : message) {
    std::vector<bool> bits(31);
    for (int i=0; i<31; i++) bits[i] = bit;
    send_packet(bits);
  }
}

std::vector<bool> receive_message(
    std::vector<std::vector<bool>> received_bits) {
  std::vector<bool> message;
  for (std::vector<bool> round : received_bits) {
    std::sort(round.begin(), round.end());
    message.push_back(round[15]);
  }
  return message;
}