cmake_minimum_required(VERSION 3.9)
project(gb17)

enable_testing()

add_subdirectory(test)

# Generic Compile flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g") # -DCPU_DEBUG=1")

# Compile application
set(shared_sources "src/CPU/ALU.cpp"
            "src/CPU/CPU.cpp"
            "src/CPU/Instructions.cpp"
            "src/MMU/Cartridge.cpp"
            "src/MMU/Cartridge_Header.cpp"
            "src/MMU/MMU.cpp"
            "src/Timer/Timer.cpp")

set(sources "src/main.cpp"
            ${shared_sources})

add_executable(gb17 ${sources})

set(state_source "src/compare.cpp"
                 ${shared_sources})

add_executable(compare ${state_source})
