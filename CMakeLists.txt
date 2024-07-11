cmake_minimum_required(VERSION 3.10)

project(LoRaGateWay VERSION 0.1)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${CMAKE_PROJECT_NAME} main.cpp AsyncSerial.cpp LoraInterface.cpp CloudGateWay.cpp)

## Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME} restclient-cpp)

set(BOOST_LIBS date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})



install (TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)