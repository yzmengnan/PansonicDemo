//
// Created by 91418 on 24-7-20.
//
#include "driver.h++"
#include "nlohmann/json.hpp"
#include <format>
#include <fstream>

using json = nlohmann::json;
int main() {
    //read json file
    std::fstream i("../config/axis_config.json");
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
        //        i.close();
    }
    auto arm_ads = new ADS::ARM_ADS();
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    m.startSYNC();
    Sleep(100);
    m.ENABLE();
    int32_t temp_position{};
    int err{};
    while (true) {
        //        if (GetAsyncKeyState(VK_UP) & 0x0001) {
        //            err = m.motionPV({1000});
        //        } else if (GetAsyncKeyState(VK_DOWN) & 0x0001) {
        //            err = m.motionPV({-1000});
        //        } else {
        //            err = m.motionPV({0});
        //        }
        //        if (err != 0) break;
        if (GetAsyncKeyState('Q') & 0x8000) { break; }

        if (GetAsyncKeyState('H') & 0x8000) {
            std::cout << "get max position" << std::endl;
            j["position_limit"]["max"] = 1000000;
        } else if (GetAsyncKeyState('L') & 0x8000) {
            std::cout << "get min position" << std::endl;
            j["position_limit"]["min"] = 200;
        }

        if (GetAsyncKeyState('S') & 0x8000) {
            std::cout << "save config data" << std::endl;
            i.seekg(0, std::ios::beg);
            i.clear();
            i << std::setw(4) << j;
            i.close();
        }

        if (abs(temp_position - m.getPosition()[0]) > 1000) {
            std::cout << "Position is: " << m.getPosition()[0] << std::endl;
            temp_position = m.getPosition()[0];
        }
        Sleep(400);
    }
    return 0;
}