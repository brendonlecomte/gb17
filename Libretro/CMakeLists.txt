cmake_minimum_required(VERSION 3.9)
include_directories(.)
include_directories(../src)
set(LINKER_SCRIPT "link.T")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

set(shared_sources  "../src/CPU/ALU.cpp"
                    "../src/CPU/CPU.cpp"
                    "../src/CPU/Instructions.cpp"
                    "../src/MMU/Cartridge.cpp"
                    "../src/MMU/Cartridge_Header.cpp"
                    "../src/MMU/MMU.cpp"
                    "../src/Timer/Timer.cpp"
                    "../src/PPU/PPU.cpp")

add_library(gb17_libretro SHARED
            libretro.cpp
            ${shared_sources})

set_target_properties(gb17_libretro PROPERTIES PREFIX "")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")
