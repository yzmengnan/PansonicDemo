//
// Created by 91418 on 24-7-24.
//

#ifndef SINGLE_DEMO_MUTEX_HPP
#define SINGLE_DEMO_MUTEX_HPP
#include "string"
#include "windows.h"
#include <iostream>
class process_mutex {
public:
    process_mutex(const std::string& s) : name{s} {
        state = false;
        hMutex = CreateMutex(NULL, TRUE, s.c_str());
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            std::cout << name << " has already run" << std::endl;
            CloseHandle(hMutex);
            state = true;
        }
    }
    ~process_mutex() { CloseHandle(hMutex); }

    bool state;

private:
    std::string name;
    HANDLE hMutex;
};

#endif//SINGLE_DEMO_MUTEX_HPP
