cmake_minimum_required(VERSION 3.0)
project(MYTHREAD)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
add_executable(mythread main.cpp)
include_directories(./)