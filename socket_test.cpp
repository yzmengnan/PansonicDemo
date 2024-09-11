//
// Created by 91418 on 24-9-10.
//
#include "boostServer.hpp"

int main() {
    boost::asio::io_context io_context;
    AsyncTcpServer t(io_context, 10010);
    io_context.run();
}
