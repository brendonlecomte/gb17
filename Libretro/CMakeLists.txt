cmake_minimum_required(VERSION 3.9)
include_directories(.)
include_directories(../src)
set(LINKER_SCRIPT "link.T")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

add_library(gb17_libretro SHARED
            libretro.cpp)

set_target_properties(gb17_libretro PROPERTIES PREFIX "")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")
