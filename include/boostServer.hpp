//
// Created by 91418 on 24-9-10.
//

#ifndef SINGLE_DEMO_BOOSTSERVER_HPP
#define SINGLE_DEMO_BOOSTSERVER_HPP

#include "winsock2.h"
//
#include "windows.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class AsyncTcpServer {
public:
    std::string command{};

public:
    AsyncTcpServer(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        command = {};
        start_accept();
    }

    void start_accept() {
        // 创建一个新的socket对象，用于接受连接
        auto *socket = new tcp::socket(acceptor_.get_executor());
        s = socket;

        // 异步等待连接，连接成功后调用handle_accept
        acceptor_.async_accept(
                *socket, boost::bind(&AsyncTcpServer::handle_accept, this, socket, boost::asio::placeholders::error));
    }

    void handle_accept(tcp::socket *socket, const boost::system::error_code &error) {
        if (!error) {
            std::cout << "New connection accepted!" << std::endl;

            // 开始异步读取数据
            socket->async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&AsyncTcpServer::handle_read, this, socket,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            std::cerr << "Error accepting connection: " << error.message() << std::endl;
            delete socket;
        }

        // 继续接受其他连接
        start_accept();
    }

    void handle_read(tcp::socket *socket, const boost::system::error_code &error, size_t bytes_transferred) {
        if (!error) {
            std::cout << "Received data: ";
            std::cout.write(data_, bytes_transferred);
            std::cout << std::endl;
            if ((char) data_[0] == 1) {
                command = "open";
            } else if ((char) data_[0] == 0) {
                command = "close";
            } else {
                command = {};
            }

            // 继续异步读取数据
            socket->async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&AsyncTcpServer::handle_read, this, socket,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            std::cerr << "Error reading data: " << error.message() << std::endl;
            delete socket;
        }
    }
    void send(const char &d) {
        char number = static_cast<char>(d);
        boost::asio::write(*s, boost::asio::buffer(&number, 1));
    }

private:
    tcp::acceptor acceptor_;
    tcp::socket *s;
    enum { max_length = 1024 };
    char data_[max_length];
};

#endif//SINGLE_DEMO_BOOSTSERVER_HPP
