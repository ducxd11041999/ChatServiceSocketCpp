cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(CBA)

include_directories(../service ../models ../Util)

file(GLOB SOURCES "../models/*.cpp" "../service/*.cpp")
find_package(Threads)
add_executable(client ${SOURCES})
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
