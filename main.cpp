#include <iostream>
#include <conio.h>

#include "driver.h++"

int main() {
    auto arm_ads = new ADS::ARM_ADS();
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    m.startSYNC();
    Sleep(1000);
    m.ENABLE();
    Sleep(2000);
    m.motionPT({100});
    Sleep(2000);
    m.motionPB({0});
    Sleep(5000);
    m.DISABLE();
    return 0;
}
