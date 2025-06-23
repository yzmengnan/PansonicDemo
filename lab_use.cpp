//
// Created by Yang on 25-3-27.
//

#include <Multi_Process.h>
#include <iostream>
#include <mutex.hpp>

#include <task.h++>

#define PLC 854
#define SLEEP_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define SLEEP_S(s) std::this_thread::sleep_for(std::chrono::seconds(s))

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

    auto t = TASK::torque_wrench(m);
    bool start = true;
    t.ApplyOscillatingTorque(start, 3s);
    // t.move_dir_1();
    // t.move_dir_0();
    // SLEEP_MS(1000);
    SLEEP_S(30);
    // for(;;)
    // {}
    start = false;
    m->DISABLE();
}