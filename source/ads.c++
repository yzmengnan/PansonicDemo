//
// Created by 91418 on 24-7-11.
//

#include <stdexcept>
#include <format>
#include "ads.h++"

namespace ADS {
    Addr::Addr() {
        AdsPortOpen();
        PAddress = nullptr;
        auto nErr = AdsGetLocalAddress(PAddress);
        if (nErr)
            throw std::runtime_error(std::format("Error: Ads get Address: {}", nErr));
    }

    Addr &Addr::getInstance() {
        static Addr instance;
        return instance;
    }

    const PAmsAddr &Addr::getAddress() {
        return PAddress;
    }

    template<typename T>
    ADS_COM<T>::ADS_COM(): addr(ads_addr::getInstance()), data(nullptr) {

    }

    template<typename T>
    int ADS_COM<T>::write() {
        //数据写入前，写设置端口，
        //针对Twincat的PLC程序，不同的PLC服务有不同的端口
        //通常为851,852...
        setPort(addr, data);
        auto nErr = AdsSyncWriteReq(addr.getAddress(), data->a, data->b, data->txSize, data->tx_data.data());
        return nErr;
    }

    template<typename T>
    int ADS_COM<T>::read() {
        setPort(addr, data);
        auto nErr = AdsSyncReadReq(addr.getAddress(), data->c, data->d, data->rxSize, data->rx_data.data());
        return nErr;
    }

    template<typename T>
    void ADS_COM<T>::setPort(ads_addr &a, T d) {
        addr.getAddress()->port = d->port;
    }

    ARM_ADS::ARM_ADS() {
        data = std::make_shared<ADS_DATA::ARM_DATA>();
        setPort(addr, data);
        m = new std::mutex;
    }

    ARM_ADS::ARM_ADS(shared_ptr<ADS_DATA::ARM_DATA> &dataPtr) {
        data = dataPtr;
        setPort(addr, data);
    }

    void ARM_ADS::startSYNC() {
        t1 = std::thread([&]() {
            while (true) {
                std::lock_guard<std::mutex> guard(*m);
                this->write();
            }
        });
        t2 = std::thread([&]() {
            while (true) {
                std::lock_guard<std::mutex> guard(*m);
                this->read();
            }
        });
        t1.detach();
        t2.detach();
    }
}