cmake_minimum_required(VERSION 3.29)
project(Single_Demo)

set(CMAKE_CXX_STANDARD 23)
include_directories(include)
find_package(nlohmann_json REQUIRED)

add_library(ADS source/ads.c++)
target_link_libraries(ADS ${PROJECT_SOURCE_DIR}/lib/TcAdsDll.lib )

add_library(driver source/driver.c++)
target_link_libraries(driver ADS nlohmann_json::nlohmann_json ws2_32 wsock32)

add_executable(Catch catch.cpp)
target_link_libraries(Catch driver)

add_executable(Wrench wrench.cpp)
target_link_libraries(Wrench driver)

add_executable(Jogging continuousJogging.cpp)
target_link_libraries(Jogging driver)

add_executable(socketTest socket_test.cpp)
target_include_directories(socketTest PUBLIC include)
target_link_libraries(socketTest PUBLIC ws2_32 wsock32)
