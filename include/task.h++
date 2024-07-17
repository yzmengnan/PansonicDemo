//
// Created by 91418 on 24-7-17.
//

#ifndef SINGLE_DEMO_TASK_H
#define SINGLE_DEMO_TASK_H

#include "driver.h++"

namespace TASK {
    class task {
    public:
        task() {
            auto arm_ads = new ADS::ARM_ADS();
            m = std::make_shared<DRIVE::ARM_DRIVE>(arm_ads);
        }

        task(std::shared_ptr<DRIVE::ARM_DRIVE> m) : m{m} {

        }

    protected:
        std::shared_ptr<DRIVE::ARM_DRIVE> m;
    };

    using tw = class torque_wrench : public task {
    private:
        enum LIMIT {
            min = -2000000,
            max = 20000000
        };
    public:
        torque_wrench() {
            std::cout << "torque wrench built" << std::endl;
        }

        int torque_screw_in() {
            std::cout << "start torque wrench tool screw in" << std::endl;
            m->setMaxSpeed({200});
            m->motionPB({100});
            if (isReached(true)) {
                return 0;
            } else {
                return torque_screw_in();
            }
        }

        int torque_screw_out() {
            std::cout << "start torque wrench tool screw out" << std::endl;
            m->setMaxSpeed({200});
            m->motionPB({100});
            if (isReached(false)) {
                return 0;
            } else {
                return torque_screw_out();
            }
        }

        bool isReached(bool dir) {
            if (dir) {
                if (this->m->getPosition()[0] > LIMIT::max) {
                    return true;
                }
                return false;
            } else {
                if (this->m->getPosition()[0] < LIMIT::min) {
                    return true;
                }
                return false;
            }
        }


    };
}

#endif //SINGLE_DEMO_TASK_H
