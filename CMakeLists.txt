cmake_minimum_required(VERSION 3.16.3)

project(hackrf_test CXX C)

INCLUDE(FindPkgConfig)
pkg_check_modules(HRF REQUIRED libhackrf)

add_executable(hackrf_test main.cpp)
message(${HRF_LIBRARIES})

target_link_libraries(hackrf_test ${HRF_LIBRARIES})
