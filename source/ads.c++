//
// Created by 91418 on 24-7-11.
//

#include "ads.h++"
#include <format>
#include <iostream>
#include <stdexcept>

namespace ADS {
    Addr::Addr() {
        AdsPortOpen();
        Address = {};
        PAddress = &Address;
        auto nErr = AdsGetLocalAddress(PAddress);
        if (nErr) throw std::runtime_error(std::format("Error: Ads get Address: {}", nErr));
    }

    Addr &Addr::getInstance() {
        static Addr instance;
        return instance;
    }

    const PAmsAddr &Addr::getAddress() { return PAddress; }

    template<typename T>
    ADS_COM<T>::ADS_COM() : addr(ads_addr::getInstance()), data(nullptr) {}

    template<typename T>
    int ADS_COM<T>::write() {
        //数据写入前，写设置端口，
        //针对Twincat的PLC程序，不同的PLC服务有不同的端口
        //通常为851,852...
        getPort(addr, data);
        auto nErr = AdsSyncWriteReq(addr.getAddress(), data->a, data->b, data->txSize, data->tx_data.data());
        return nErr;
    }

    template<typename T>
    int ADS_COM<T>::read() {
        getPort(addr, data);
        auto nErr = AdsSyncReadReq(addr.getAddress(), data->c, data->d, data->rxSize, data->rx_data.data());
        //        std::cout<<data->rx_data[0].status_word<<std::endl;
        return nErr;
    }

    axis_ads::axis_ads() {
        data = std::make_shared<ADS_DATA::ARM_DATA>();
        getPort(addr, data);
        m = new std::mutex;
    }

    axis_ads::axis_ads(shared_ptr<ADS_DATA::ARM_DATA> &dataPtr) {
        data = dataPtr;
        getPort(addr, data);
    }

    void axis_ads::startSYNC() {
        t1 = std::thread([&]() {
            while (true) {
                //                std::lock_guard<std::mutex> guard(*m);
                this->write();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
        t2 = std::thread([&]() {
            while (true) {
                //                std::lock_guard<std::mutex> guard(*m);
                this->read();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
        t1.detach();
        t2.detach();
    }
}// namespace ADS
