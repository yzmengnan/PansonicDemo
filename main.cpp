#include <iostream>
#include <conio.h>

#include "driver.h++"
#include "task.h++"

int main() {
    auto arm_ads = new ADS::ARM_ADS();
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    //start sync data
    m.startSYNC();
    Sleep(1000);
    //servo enable
    m.ENABLE();
    Sleep(2000);
    //use profile torque mode
    m.motionPT({100});
    Sleep(2000);
    //set rpm=100
    m.setProfileVelocity({100});
    //use profile position mode
    m.motionPB({0});
    Sleep(5000);
    //servo disable
    m.DISABLE();
    auto t = TASK::torque_wrench();
    t.torque_screw_out();
    return 0;
}
