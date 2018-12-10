cmake_minimum_required(VERSION 3.12)
project(gb17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")


set(sources "main.cpp")

add_executable(gb17 ${sources})
