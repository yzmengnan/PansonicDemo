#include <conio.h>
#include <iostream>

#include "driver.h++"
#include "nlohmann/json.hpp"
#include "task.h++"
#include <fstream>

using json = nlohmann::json;

int main(int argc, char **argv) {
    auto arm_ads = new ADS::ARM_ADS();
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    //start sync data
    m.startSYNC();
    Sleep(1000);
    //servo enable
    m.ENABLE();
    std::shared_ptr<DRIVE::ARM_DRIVE> m_ptr = make_shared<DRIVE::ARM_DRIVE>(m);
    std::fstream js_file("../config/axis_config.json");
    json j;
    js_file >> j;
    if (!js_file.is_open()) {
        std::cerr << "error! json config file not founded" << std::endl;
        return -1;
    }
    auto t = TASK::torque_wrench(m_ptr, (int) j["position_limit"]["max"], (int) j["position_limit"]["min"],
                                 (short) j["torque_limit"]["max"], (short) j["torque_limit"]["min"]);

    if (argc >= 2) {
        auto signal = atoi(argv[1]);
        if (signal == 1) return t.move_dir_0();
        else if (signal == 0)
            return t.move_dir_1();
    }
    return 0;
}
