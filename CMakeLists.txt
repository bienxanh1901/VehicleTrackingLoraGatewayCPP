cmake_minimum_required(VERSION 3.10)

project(LoRaGateWay VERSION 0.1)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${CMAKE_PROJECT_NAME} main.cpp LoraInterface.cpp CloudGateWay.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} restclient-cpp)