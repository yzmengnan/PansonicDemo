//
// Created by 91418 on 24-7-11.
//

#ifndef SINGLE_DEMO_DRIVER_H
#define SINGLE_DEMO_DRIVER_H


#include <iostream>
#include "ads.h++"

using namespace std;

namespace DRIVE {
    class ARM_DRIVE {
    private:
        enum OP_MODE {
            PP = 1,
            PT = 4
        };
    public:
        ARM_DRIVE() = delete;

        ~ARM_DRIVE() {
            std::cerr << "ARM DRIVE QUIT!" << std::endl;
            this->DISABLE();
        }

        explicit ARM_DRIVE(ADS::ARM_ADS *ads);

        void startSYNC() {
            this->ads_Handle->startSYNC();
            this->syncFlag = true;
        }

        void clearFault();

        int ENABLE();

        void DISABLE();

        int setOperationMode(const DRIVE::ARM_DRIVE::OP_MODE &pMode);

        void servoBreak(bool b);

        int motionPB(initializer_list<int32_t> targetPosition);

        int motionPT(initializer_list<int16_t> targetTorque);

    private:
        ADS::ARM_ADS *ads_Handle;
        bool syncFlag;
        bool enableFlag;
        int mode;
        std::vector<ADS_DATA::servo_tx> &Tx;
        std::vector<ADS_DATA::servo_rx> &Rx;
    };
}


#endif //SINGLE_DEMO_DRIVER_H
