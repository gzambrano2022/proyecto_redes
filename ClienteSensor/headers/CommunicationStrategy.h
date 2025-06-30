#ifndef COMMUNICATION_STRATEGY_H
#define COMMUNICATION_STRATEGY_H

#include <cstdint>
#include <vector>

class CommunicationStrategy {
public:
    virtual ~CommunicationStrategy() = default; // destructor polimórfico
    virtual std::vector<uint8_t> encode(const std::vector<uint8_t>& data) = 0;
    virtual bool send(const std::vector<uint8_t>& data) = 0;
};

#endif // COMMUNICATION_STRATEGY_H