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

        task(std::shared_ptr<DRIVE::ARM_DRIVE> m) : m{m} {}

    protected:
        std::shared_ptr<DRIVE::ARM_DRIVE> m;
    };

    using tw = class torque_wrench : public task {
    private:
        enum LIMIT { max = -16134627, min = -7838708 };
        enum DIR { forward = false, backward = true };

    public:
        torque_wrench() { std::cout << "torque wrench built" << std::endl; }
        torque_wrench(std::shared_ptr<DRIVE::ARM_DRIVE> m_ptr) : task(m_ptr) { std::cout << "torque wrench built" << std::endl; }

        int torque_screw_in() {
            std::cout << "start torque wrench tool screw in" << std::endl;
            m->setMaxSpeed({600});
            short torque_value{300};
            while (isReached(DIR::forward)) {
                m->motionPT({torque_value});
                if (torque_value <= 1000) {
                    torque_value += 20;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
            std::cout<<"reached catch position"<<std::endl;
            m->DISABLE();
            return 0;
        }

        int torque_screw_out() {
            std::cout << "start torque wrench tool screw out" << std::endl;
            m->setMaxSpeed({1000});
            short torque_value{-700};
            while (isReached(DIR::backward)) {
                m->motionPT({torque_value});
                if (torque_value >= -1000) {
                    torque_value -= 20;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
            std::cout<<"reached release position"<<std::endl;
            m->DISABLE();
            return 0;
        }

        bool isReached(DIR dir) {
            return dir ? this->m->getPosition()[0] >= LIMIT::max : this->m->getPosition()[0] <= LIMIT::min;
        }
    };
}// namespace TASK

#endif//SINGLE_DEMO_TASK_H
