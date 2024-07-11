#include <iostream>

#include "driver.h++"

int main() {
    auto arm_ads = new ADS::ARM_ADS();
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    m.startSYNC();
    m.ENABLE();
    Sleep(20000);
    return 0;
}
