//
// Created by 91418 on 24-7-20.
//
#include "driver.h++"
#include "nlohmann/json.hpp"
#include <format>
#include <fstream>

using json = nlohmann::json;
int main(int argc, char **argv) {
    //read json file
    if (argc != 3) { return -3; }
    std::string json_name = argv[1];
    int port = stoi(argv[2]);
    std::ifstream i(json_name);
    json j;
    i >> j;
    if (!i.is_open()) {
        std::cerr << "error! no 'axis_config.json' file found" << std::endl;
        return -1;
    } else {
        //check data
        std::cout << std::format("Read config file: \n"
                                 "joint limit:{}, {}, torque set:{}, {}",
                                 (int) j["position_limit"]["max"], (int) j["position_limit"]["min"],
                                 (int) j["torque_limit"]["max"], (int) j["torque_limit"]["min"])
                  << std::endl;
        i.close();
    }
    auto arm_ads = new ADS::ARM_ADS();
    arm_ads->setPort(port);
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    m.startSYNC();
    Sleep(100);
    m.ENABLE();
    int32_t temp_position{};
    int err{};
    while (true) {
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            err = m.motionPV({6000});
        } else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            err = m.motionPV({-6000});
        } else {
            err = m.motionPV({0});
        }
        if (err != 0) break;
        if (GetAsyncKeyState('Q') & 0x8000) { break; }

        if (GetAsyncKeyState('H') & 0x8000) {
            std::cout << "get max position" << std::endl;
            j["position_limit"]["max"] = m.getPosition()[0];
        } else if (GetAsyncKeyState('L') & 0x8000) {
            std::cout << "get min position" << std::endl;
            j["position_limit"]["min"] = m.getPosition()[0];
        }

        if (GetAsyncKeyState('S') & 0x8000) {
            std::cout << "save config data" << std::endl;
            std::ofstream o(json_name);
            o << std::setw(4) << j;
            o.close();
        }

        if (abs(temp_position - m.getPosition()[0]) > 1000) {
            std::cout << "Position is: " << m.getPosition()[0] << std::endl;
            temp_position = m.getPosition()[0];
        }
        //        Sleep(400);
    }
    return 0;
}