cmake_minimum_required(VERSION 3.29)
project(Single_Demo)

set(CMAKE_CXX_STANDARD 23)
include_directories(include)

add_library(ADS source/ads.c++)
target_link_libraries(ADS ${PROJECT_SOURCE_DIR}/lib/TcAdsDll.lib )

add_library(driver source/driver.c++)
target_link_libraries(driver ADS)

add_executable(Single_Demo main.cpp)
target_link_libraries(Single_Demo driver)

add_executable(Jogging continuousJogging.cpp)
target_link_libraries(Jogging driver)
