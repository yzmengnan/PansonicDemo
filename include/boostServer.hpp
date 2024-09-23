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
    std::string command{-1};

public:
    AsyncTcpServer(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        command = {};
        socket_ = nullptr;
        start_accept();
    }

    void start_accept() {
        // 创建一个新的socket对象，用于接受连接
        auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());

        // 异步等待连接，连接成功后调用handle_accept
        acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code &error) {
            this->handle_accept(socket, error);
        });
    }

    void handle_accept(const std::shared_ptr<tcp::socket> &socket, const boost::system::error_code &error) {
        if (!error) {
            std::cout << "New connection accepted!" << std::endl;
            socket_ = socket;
            // 开始异步读取数据
            socket->async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&AsyncTcpServer::handle_read, this, socket.get(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            std::cerr << "Error accepting connection: " << error.message() << std::endl;
        }

        // 继续接受其他连接
        start_accept();
    }

    void handle_read(tcp::socket *socket, const boost::system::error_code &error, size_t bytes_transferred) {
        if (!error) {
            std::cout << "Received data: ";
            std::cout.write(data_, bytes_transferred);
            std::cout << std::endl;
            if ((char) data_[0] == 0) {
                std::cout << "Received Command: open" << std::endl;
                command = "close";
            } else if ((char) data_[0] == 1) {
                std::cout << "Received Command: close" << std::endl;
                command = "open";
            } else if ((char) data_[0] == 2) {
                command = "rotate_r";
            } else if ((char) data_[0] == 3) {
                command = "rotate_l";
            } else if ((char) data_[0] == 4) {
                command = "clear";
            } else if ((char) data_[0] == 5) {
                command = "disable";
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
            //            delete socket;
            socket_ = nullptr;
        }
    }
    void send(const char d) {
        if (socket_ != nullptr) {
            if (socket_->is_open()) {
                std::cout << "send " << std::to_string(d) << std::endl;
                boost::asio::write(*socket_, boost::asio::buffer(&d, 1));
            } else {
                std::cerr << "client not connected!" << std::endl;
            }
        }
    }
    void send(const std::string& s) {
        if (socket_ != nullptr) {
            if (socket_->is_open()) {
                boost::asio::write(*socket_, boost::asio::buffer(s));
            } else {
                std::cerr << "client not connected!" << std::endl;
            }
        }
    }

private:
    tcp::acceptor acceptor_;
    std::shared_ptr<tcp::socket> socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

#endif//SINGLE_DEMO_BOOSTSERVER_HPP
