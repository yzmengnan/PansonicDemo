#include <conio.h>
#include <iostream>

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
    if(argc!=4){
        return -3;
    }
    std::string json_name = argv[3];
    std::fstream js_file(json_name);
    json j;
    js_file >> j;
    if (!js_file.is_open()) {
        std::cerr << "error! json config file not founded" << std::endl;
        return -1;
    }
    auto arm_ads = new ADS::ARM_ADS();
    
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

    if (argc >= 2) {
        auto signal = atoi(argv[1]);
        if (signal == 1) return t.move_dir_0();
        else if (signal == 0)
            return t.move_dir_1();
    }
    return 0;
}
