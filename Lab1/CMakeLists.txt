cmake_minimum_required(VERSION 3.17)
project(ACS_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

add_executable(ACS_1 main.cpp)