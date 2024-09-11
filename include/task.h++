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

        explicit task(std::shared_ptr<DRIVE::ARM_DRIVE> m) : m{m} {}

        ~task() { std::cout << "task object delete" << std::endl; }

    protected:
        std::shared_ptr<DRIVE::ARM_DRIVE> m;
    };

    using tw = class torque_wrench : public task {
    private:
        enum LIMIT { max = -16134627, min = -7838708 };
        enum DIR { forward = false, backward = true };
        int limit_max{};
        int limit_min{};
        short torque_dir_0;
        short torque_dir_1;

    public:
        torque_wrench() {
            std::cout << "torque wrench built" << std::endl;
            limit_max = LIMIT::max;
            limit_min = LIMIT::min;
            torque_dir_0 = 300;
            torque_dir_1 = -1000;
        }
        explicit torque_wrench(std::shared_ptr<DRIVE::ARM_DRIVE> m_ptr) : task(m_ptr) {
            std::cout << "torque wrench built" << std::endl;
            limit_max = LIMIT::max;
            limit_min = LIMIT::min;
            torque_dir_0 = 300;
            torque_dir_1 = -1000;
        }
        torque_wrench(std::shared_ptr<DRIVE::ARM_DRIVE> m_ptr, int a, int b, short c, short d) : task(m_ptr) {
            std::cout << "torque wrench built" << std::endl;
            limit_max = a;
            limit_min = b;
            torque_dir_0 = c;
            torque_dir_1 = d;
        }

        ~torque_wrench() { std::cout << "torque wrench tool delete" << std::endl; }

        //move cw
        int move_dir_0() {
            m->setMaxSpeed({2000});
            short torque_value{400};
            auto last_position = m->getPosition()[0];
            size_t counts{};
            while (isReached(DIR::forward)) {
                counts += (m->motionPT({torque_value}) != 0);
                if (torque_value <= this->torque_dir_0) { torque_value += 20; }
                counts += abs(last_position - m->getPosition()[0]) <= 1000;
                last_position = m->getPosition()[0];
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                if (counts >= 100) {
                    std::cout << "stack!" << std::endl;
                    m->DISABLE();
                    return -300;
                }
            }
            std::cout << "reached limited max position" << std::endl;
            m->DISABLE();
            return 0;
        }

        //move ccw
        int move_dir_1() {
            m->setMaxSpeed({2000});
            short torque_value{-200};
            auto last_position = m->getPosition()[0];
            size_t counts{};
            while (isReached(DIR::backward)) {
                m->motionPT({torque_value});
                if (torque_value >= this->torque_dir_1) {
                    torque_value -= 20;
                }
                counts += abs(last_position - m->getPosition()[0]) <= 1000;
                last_position = m->getPosition()[0];
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                if (counts >= 100) {
                    std::cout << "stack!" << std::endl;
                    m->DISABLE();
                    return 1;
                }
            }
            std::cout << "reached limited min position" << std::endl;
            m->DISABLE();
            return 0;
        }

        bool isReached(DIR dir) {
            return !dir ? this->m->getPosition()[0] < this->limit_max : this->m->getPosition()[0] > this->limit_min;
        }
    };
}// namespace TASK

#endif//SINGLE_DEMO_TASK_H
