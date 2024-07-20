//
// Created by 91418 on 24-7-20.
//
#include "driver.h++"

int main() {
    auto arm_ads = new ADS::ARM_ADS();
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    m.startSYNC();
    Sleep(100);
    m.ENABLE();
    int32_t temp_position{};
    while (true) {
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            m.motionPV({1000});
        } else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            m.motionPV({-1000});
        } else {
            m.motionPV({0});
        }
        if (GetAsyncKeyState('Q') & 0x8000) { break; }
        if (abs(temp_position - m.getPosition()[0]) > 1000) {
            std::cout << "Position is: " << m.getPosition()[0] << std::endl;
            temp_position = m.getPosition()[0];
        }
    }
    return 0;
}