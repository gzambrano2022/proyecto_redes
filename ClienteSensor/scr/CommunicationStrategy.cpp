#include <cstdint>
#include <vector>
#include "CommunicationStrategy.h"

class CommunicationStrategy {
    public:
        virtual ~CommunicationStrategy()  = default; // constructor polimorfico
        virtual std::vector<uint8_t> encode(const std::vector<uint8_t>& data) = 0;
        virtual bool send(const std::vector<uint8_t>& data) = 0;

};