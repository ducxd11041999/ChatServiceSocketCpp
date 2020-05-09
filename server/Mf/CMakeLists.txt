cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(CBA)

include_directories(../service ../include ../responsibility ../config ../models ../Util)

file(GLOB SOURCES "../models/*.cpp" "../service/*.cpp" "../Util/*.cpp" "../responsibility/*.cpp")

add_executable(server ${SOURCES})
target_link_libraries(server mysqlcppconn)
#target_link_libraries(server curl mysql--connector-c++)

