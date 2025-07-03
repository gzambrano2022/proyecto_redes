#include "headers/CommunicationStrategy.h"
#include <string>
#include <vector>
#include <cstdint>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
using boost::asio::ip::tcp;

class StrategyTCP : public CommunicationStrategy {
    private:
        std::string ServerIP;
        int ServerPort;
        boost::asio::io_context IOContext;
        std::unique_ptr<boost::asio::ip::tcp::socket> Socket;
    public:
        StrategyTCP(const std::string& ip, int port)
            : ServerIP(ip), ServerPort(port), Socket(-1) {
            // Initialize the socket here if needed
        }

};