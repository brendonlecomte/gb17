cmake_minimum_required(VERSION 3.9)
project(gb17)

enable_testing()

add_subdirectory(test)

# Generic Compile flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

# Compile application
set(sources "src/main.cpp"
            "src/alu.cpp"
            "src/cpu.cpp")

add_executable(gb17 ${sources})
