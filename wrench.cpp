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
        std::cout << "USE SOCKET COMMAND" << std::endl;
        //using socket command
        boost::asio::io_context IO;
        AsyncTcpServer *asyncTcp;
        //for torque wrench catch tool
        if (plc_number == 854) {
            asyncTcp = new AsyncTcpServer(IO, 10020);
            std::thread thread_IO([&]() { IO.run(); });
            static bool isShown = false;
            static bool isDisabled = false;
            int no_message_numbers = 0;
            while (true) {
                if (asyncTcp->command == "rotate_l") {
                    std::cout << "rotate_l" << std::endl;
                    isShown = false;
                    m->ENABLE();
                    m->setMaxSpeed({6000});
                    m->motionPT({500});
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    asyncTcp->command = {};
                    isDisabled = false;
                } else if (asyncTcp->command == "rotate_r") {
                    std::cout << "rotate_r" << std::endl;
                    isShown = false;
                    m->ENABLE();
                    m->setMaxSpeed({6000});
                    m->motionPT({-500});
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    asyncTcp->command = {};
                    isDisabled = false;
                } else if (asyncTcp->command == "disable") {
                    isShown = false;
                    m->DISABLE();
                    asyncTcp->command = {};
                    isDisabled = true;
                } else {
                    no_message_numbers++;
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    if (no_message_numbers == 10) {

                        if (!isShown) {
                            std::cout << "wait for command" << std::endl;
                            isShown = true;
                        }

                        if (!isDisabled) m->motionPT({0});
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(20));

                auto t_value = m->getTorque()[0];
                auto p_value = m->getPosition()[0];
                wrench_Data w = {t_value, p_value};
                asyncTcp->send(w);
                //clear the position
                if (asyncTcp->command == "clear") {}
            }
            thread_IO.detach();
        } else {
            asyncTcp = nullptr;
            std::cout << "error! invalid plc port" << std::endl;
            return -1;
        }
    }
    return 0;
}
