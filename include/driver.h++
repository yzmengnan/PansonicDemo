//
// Created by 91418 on 24-7-11.
//

#ifndef SINGLE_DEMO_DRIVER_H
#define SINGLE_DEMO_DRIVER_H

#define rpm2P 8388608 / 60.0f
#include "ads.h++"
#include <iostream>

using namespace std;

namespace DRIVE {
    class axis_drive {
    private:
        enum OP_MODE { PP = 1, PV = 3, PT = 4 };

    public:
        axis_drive() = delete;

        ~axis_drive() {
            std::cerr << "ARM DRIVE QUIT!" << std::endl;
            this->DISABLE();
        }

        explicit axis_drive(ADS::axis_ads *ads);

        void startSYNC() {
            this->ads_Handle->startSYNC();
            this->syncFlag = true;
        }

        bool clearFault();

        int ENABLE();

        void DISABLE();

        void HALT(bool halt);

        void setProfileVelocity(initializer_list<int> rpm);

        void setMaxSpeed(initializer_list<int> rpm);

        int setOperationMode(const DRIVE::axis_drive::OP_MODE &pMode);

        void servoBreak(bool b);

        int motionPB(initializer_list<int32_t> targetPosition);

        int motionPT(initializer_list<int16_t> targetTorque);

        int motionPV(initializer_list<int32_t> targetVelocity);

        std::vector<int> getPosition();

        std::vector<int> getTorque();

        std::vector<int> getVelocity();

    private:
        ADS::axis_ads *ads_Handle;
        bool syncFlag;
        bool enableFlag;
        int mode;
        std::vector<ADS_DATA::servo_tx> &Tx;
        std::vector<ADS_DATA::servo_rx> &Rx;
    };
}// namespace DRIVE


#endif//SINGLE_DEMO_DRIVER_H
