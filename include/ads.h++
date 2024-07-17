//
// Created by 91418 on 24-7-11.
//

#ifndef SINGLE_DEMO_ADS_H
#define SINGLE_DEMO_ADS_H

#include <vector>
#include <cstdint>
#include <memory>
#include <thread>
#include "windows.h"
#include "TcAdsDef.h"
//
#include "TcAdsAPI.h"
namespace ADS_DATA{
    class ARM_DATA;
}

namespace ADS {
    using namespace std;
    /**
     * \brief： addr单例对象，构建ADS通讯通道
     */
    using ads_addr = class Addr {
    private:
        /**
         * \brief 打开ADS通讯端口
         */
        Addr();

        ~Addr() {
            AdsPortClose();
        }
        AmsAddr Address;
        PAmsAddr PAddress;
    public:
        Addr(const Addr &) = delete;

        Addr &operator=(const Addr &) = delete;

        /**
         * \brief 单例模式，只有一个ADS的通讯对象被建立。通常
         * 即使多个ADS通讯被建立，实际使用的端口仍然只有一个，
         * 单例模式保证了再一个进程内，ADS的端口是唯一的。
         * *多进程开发过程中的，ADS端口问题未确认。
         */
        static Addr &getInstance();

        /**
         * \brief: 获取ADS端口的ads地址，这个地址用来确立数据传输中的ADS协议IP
         */
        const PAmsAddr &getAddress();
    };

    /**
     * \brief: ADS的通讯基类
     * \param: T:ADS端口通讯时不同的数据类型
     */
    template<typename T>
    class ADS_COM {
    public:
        ADS_COM();

        int write();

        int read();

        void setPort(ads_addr &a, T d);

        T data;
    protected:
        ads_addr &addr;
    };

    class ARM_ADS : public ADS_COM<std::shared_ptr<ADS_DATA::ARM_DATA>> {
    public:
        //内部建立一个数据对象
        ARM_ADS();

        //使用外部数据
        explicit ARM_ADS(std::shared_ptr<ADS_DATA::ARM_DATA>& dataPtr);

        void startSYNC();
    private:
        std::thread t1;
        std::thread t2;
        std::mutex* m ;
    };

}


namespace ADS_DATA {
    using namespace std;

    enum nums {
        driver_counts = 1
    };
    enum PLC_PORT {
        port1 = 851,
        port2
    };

    enum adsAddress {
        output_base = 0xf030,
        output_offset = 0x3e800,
        input_base = 0xf020,
        input_offset = 0x1f400,
    };
    enum dataSize {
        data_tx_size = 24,
        data_rx_size = 20,
    };


    class DataType {
    public:
        uint64_t a{};
        uint64_t b{};
        uint64_t c{};
        uint64_t d{};
        int txSize{};
        int rxSize{};
        int port{PLC_PORT::port1};
    };

    struct servo_tx {
        uint16_t control_word = 0;
        int32_t target_position = 0;
        uint32_t profile_velocity = 0x7fffffff;
        uint32_t max_velocity = 3000;
        int8_t operation_mode = 1;
        int16_t target_torque = 0;
        uint16_t max_torque = 1500;
        uint16_t null_space = 0;
    };
    struct servo_rx {
        uint16_t status_word = 0;
        int8_t operation_mode_display = 0;
        int32_t actual_position = 0;
        int32_t actual_velocity = 0;
        int32_t following_error = 0;
        int16_t actual_torque = 0;
    };

    class ARM_DATA : public DataType {
    public:
        ARM_DATA() {
            a = adsAddress::output_base;
            b = adsAddress::output_offset;
            c = adsAddress::input_base;
            d = adsAddress::input_offset;
            tx_data = vector<servo_tx>(driver_counts);
            rx_data = vector<servo_rx>(driver_counts);
            txSize = (int) tx_data.size() * dataSize::data_tx_size;
            rxSize = (int) rx_data.size() * dataSize::data_rx_size;
            port = PLC_PORT::port1;
        }

        vector<servo_tx> tx_data;
        vector<servo_rx> rx_data;

    };
}

#endif //SINGLE_DEMO_ADS_H
