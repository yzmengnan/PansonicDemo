#include <conio.h>
#include <iostream>

#include "driver.h++"
#include "task.h++"

int main(int argc, char **argv) {
    auto arm_ads = new ADS::ARM_ADS();
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    //start sync data
    m.startSYNC();
    Sleep(1000);
    //servo enable
    m.ENABLE();
    std::shared_ptr<DRIVE::ARM_DRIVE> m_ptr = make_shared<DRIVE::ARM_DRIVE>(m);
    auto t = TASK::torque_wrench(m_ptr);
    if (argc >= 2) {
        auto signal = atoi(argv[1]);
        if (signal == 1) return t.torque_screw_in();
        else if (signal == 0)
            return t.torque_screw_out();
    }
    return 0;
}
