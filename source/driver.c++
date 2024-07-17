//
// Created by 91418 on 24-7-11.
//

#include <format>
#include "driver.h++"

DRIVE::ARM_DRIVE::ARM_DRIVE(ADS::ARM_ADS *ads) : ads_Handle{ads}, Tx{ads_Handle->data->tx_data},
                                                 Rx{ads_Handle->data->rx_data} {
    syncFlag = false;
    enableFlag = false;
    mode = {};
    cout << "ARM DRIVE BUILT!" << endl;
}

bool DRIVE::ARM_DRIVE::clearFault() {
    for (auto &c: this->Tx) { c.control_word |= 0x80; }
    this->ads_Handle->write();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (auto &c: this->Tx) { c.control_word &= ~0x80; }
    this->ads_Handle->write();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    this->ads_Handle->read();
    size_t cnts{};
    for (const auto &r: this->Rx) {
        cnts += (r.status_word & 0b1000) >> 3;
    }
    return cnts >= 1;
}

int DRIVE::ARM_DRIVE::ENABLE() {
    if (!syncFlag) {
        cerr << "please start transform data" << endl;
        return -1003;
    }
    if (this->enableFlag) { return 0; }
    // try 3 times
    for (int try_count = 0; try_count < 3; try_count++) {
        uint8_t state = 0;
        // 首先检查是否已经使能
        for (auto child: Rx) { state += static_cast<int>((child.status_word &= 0x37) == 0x37); }
        if (state == ADS_DATA::nums::driver_counts) {
            cerr << "All servo has been enabled!" << '\n';
            // 如果已经使能了，需要更新当前控制字的引用
            for (auto &child: Tx) { child.control_word |= 0x000f; }
            enableFlag = true;
            return 0;
        }
        for (auto child_servo: Rx) { state += static_cast<int>((child_servo.status_word &= 0x40) == 0x40); }
        if (state == ADS_DATA::nums::driver_counts) {
            state = 0;
        } else {
            cerr << "Servo Enable trying, time_counts:" << try_count + 1 << '\n';
            continue;
        }
        for (auto &child_servo: Tx) { child_servo.control_word = 0x0006; }
        this_thread::sleep_for(chrono::milliseconds(200));
        for (auto child_servo: Rx) { state += static_cast<int>((child_servo.status_word &= 0x21) == 0x21); }
        if (state == ADS_DATA::nums::driver_counts) {
            state = 0;
        } else {
            cerr << "Servo Enable trying, time_counts:" << try_count + 1 << '\n';
            continue;
        }
        for (auto &child_servo: Tx) { child_servo.control_word = 0x0007; }
        this_thread::sleep_for(chrono::milliseconds(200));
        for (auto child_servo: Rx) { state += static_cast<int>((child_servo.status_word &= 0x23) == 0x23); }
        if (state == ADS_DATA::nums::driver_counts) {
            state = 0;
        } else {
            cerr << "Servo Enable trying, time_counts:" << try_count + 1 << '\n';
            continue;
        }
        for (auto &child_servo: Tx) { child_servo.control_word = 0x000F; }
        this_thread::sleep_for(chrono::milliseconds(100));
        for (auto child_servo: Rx) { state += static_cast<int>((child_servo.status_word &= 0x37) == 0x37); }
        if (state == ADS_DATA::nums::driver_counts) {
            cerr << "All Servos Operation enabled!" << '\n';
            servoBreak(false);
            this_thread::sleep_for(chrono::milliseconds(120));
            enableFlag = true;
            return 0;
        }
        cerr << "Servo Enable trying, time_counts:" << try_count + 1 << '\n';
        this->clearFault();
        this_thread::sleep_for(chrono::seconds(2));
    }
    servoBreak(false);
    cerr << "Servo Enable Failure......" << '\n';
    return -1002;

}

void DRIVE::ARM_DRIVE::DISABLE() {
    servoBreak(false);
    mode = 0;
    for (auto &child_servo: Tx) {
        child_servo.operation_mode = 1;
        child_servo.control_word = 0;
    }
    ads_Handle->write();
    std::cerr << "All Servos Operation disabled!" << '\n';
    enableFlag = false;
}

int DRIVE::ARM_DRIVE::setOperationMode(const DRIVE::ARM_DRIVE::OP_MODE &pMode) {
    if (mode == pMode) {
        return 0;
    }
    if (this->enableFlag) {
        this->DISABLE();
    }
    for (auto &d: Tx) {
        d.operation_mode = pMode;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    size_t check_counts{};
    for (const auto &r: Rx) {
        if (r.operation_mode_display == pMode) {
            check_counts++;
        }
    }
    if (check_counts == ADS_DATA::nums::driver_counts) {
        std::cout << std::format("operation mode:{} change successfully!", (int) pMode) << std::endl;
        this->ENABLE();
        mode = pMode;
        return 0;
    } else {
        std::cout << "set operation mode failure! try again!" << std::endl;
        return setOperationMode(pMode);
    }
}

void DRIVE::ARM_DRIVE::servoBreak(bool b) {

}

int DRIVE::ARM_DRIVE::motionPB(initializer_list<int32_t> targetPosition) {
    auto err = setOperationMode(OP_MODE::PP);
    if (err == 0) {
        size_t i{};
        for (const auto &t: targetPosition) {
            if (i > ADS_DATA::nums::driver_counts) {
                return 1;
            }
            Tx[i++].target_position = t;
        }
        for (auto &d: Tx) {
            d.control_word |= 0x10;
        }
        this_thread::sleep_for(std::chrono::milliseconds(100));
        int statusReadyCount{};
        for (const auto &child_servo: Rx) {
            if ((child_servo.status_word & 0x1000) != 0) statusReadyCount++;
        }
        // 如果是伺服均收到新的坐标位置，更新控制字，准备下一次位置更新
        if (statusReadyCount == ADS_DATA::nums::driver_counts) {
            for (auto &child_servo: Tx) {
                child_servo.control_word &= 0xffef;// 控制字BIT4置0
            }
            return 0;
        }
        std::cerr << "servo lagged!" << std::endl;
        return -3001;
    } else {
        return err;
    }
}

int DRIVE::ARM_DRIVE::motionPT(initializer_list<int16_t> targetTorque) {
    auto err = setOperationMode(OP_MODE::PT);
    if (err == 0) {
        size_t i{};
        for (const auto &t: targetTorque) {
            if (i > ADS_DATA::nums::driver_counts) {
                return 1;
            }
            Tx[i++].target_torque = t;
        }
        return 0;
    } else {
        return err;
    }

}

void DRIVE::ARM_DRIVE::setProfileVelocity(initializer_list<int> rpm) {
    std::vector<int> rpm_pulses = {};
    for (const auto &r: rpm) {
        rpm_pulses.push_back(static_cast<int>((8388608 / 60.0f) * (float) r));
    }
    size_t i{};
    for (auto &r: Tx) {
        r.profile_velocity = rpm_pulses[i++];
    }
}

