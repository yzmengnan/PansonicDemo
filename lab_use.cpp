//
// Created by Yang on 25-3-27.
//

#include <Multi_Process.h>
#include <iostream>
#include <mutex.hpp>

#include <task.h++>

#define PLC 854
#define SLEEP_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))

int main(int argc, char **argv) {



    std::string name = "exe";

    auto mut = process_mutex(name);

    if (mut.state) return -2;

    Multi_Process s;
    auto pi = s.safety_monitor_build("SAFE_CHECK.exe");

    const auto arm_ads = new ADS::axis_ads();

    arm_ads->setPort(PLC);

    const auto m = std::make_shared<DRIVE::axis_drive>(arm_ads);

    m->startSYNC();
    SLEEP_MS(100);

    m->ENABLE();
    m->setMaxSpeed({2000});
    m->motionPT({200});

    SLEEP_MS(10000);
    m->DISABLE();

}