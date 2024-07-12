#include <iostream>
#include <conio.h>

#include "driver.h++"

int main() {
    auto arm_ads = new ADS::ARM_ADS();
    auto m = DRIVE::ARM_DRIVE(arm_ads);
    m.startSYNC();
    m.ENABLE();
    while (true) {
        if (_kbhit()) {
            auto kb = _getch();
            if (kb == 27)
                break;
            else if (kb == 38) {
                m.motionPT({100});
            } else if (kb == 40) {
                m.motionPT({-100});
            } else {
                Sleep(200);
                continue;
            }
        }
    }
    m.DISABLE();
    return 0;
}
