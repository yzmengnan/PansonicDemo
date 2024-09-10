#include <conio.h>
#include <iostream>

#include "boostServer.hpp"
//
#include "driver.h++"
#include "mutex.hpp"
#include "nlohmann/json.hpp"
#include "task.h++"
#include <fstream>
using json = nlohmann::json;


int main(int argc, char **argv) {
    std::string name;
    if (argc >= 3) {
        name = argv[2];
    } else {
        name = "exe1";
    }
    auto mut = process_mutex(name);
    if (mut.state) { return -2; }
    if (argc != 5) { return -3; }
    auto plc_number = atoi(argv[4]);
    std::string json_name = argv[3];
    std::fstream js_file(json_name);
    json j;
    js_file >> j;
    if (!js_file.is_open()) {
        std::cerr << "error! json config file not founded" << std::endl;
        return -1;
    }
    auto arm_ads = new ADS::ARM_ADS();
    arm_ads->setPort(plc_number);
    //set address
    //    arm_ads->setAddress(0,0,0,0);

    auto m = std::make_shared<DRIVE::ARM_DRIVE>(arm_ads);
    //start sync data
    m->startSYNC();
    Sleep(1000);
    //servo enable
    m->ENABLE();
    auto t = TASK::torque_wrench(m, (int) j["position_limit"]["max"], (int) j["position_limit"]["min"],
                                 (short) j["torque_limit"]["max"], (short) j["torque_limit"]["min"]);

    if (std::getenv("USE_SOCKET_COMMAND") == nullptr) {
        // not using socket command
        if (argc >= 2) {
            auto signal = atoi(argv[1]);
            if (signal == 1) return t.move_dir_0();
            else if (signal == 0)
                return t.move_dir_1();
        }
    } else {
        std::cout<<"USE SOCKET COMMAND"<<std::endl;
        //using socket command
        boost::asio::io_context IO;
        AsyncTcpServer asyncTcp(IO, 10010);
        IO.run();
        while (true) {
            if (asyncTcp.command == "close") {
                m->ENABLE();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                auto res = t.move_dir_0();
                if(res!=0){
                    asyncTcp.send(0);
                }
                else{
                    asyncTcp.send(1);
                }
            } else if (asyncTcp.command == "open") {
                m->ENABLE();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                auto res = t.move_dir_1();
                if(res!=0){
                    asyncTcp.send(1);
                }
                else{
                    asyncTcp.send(0);
                }
            } else {
                m->DISABLE();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    return 0;
}
