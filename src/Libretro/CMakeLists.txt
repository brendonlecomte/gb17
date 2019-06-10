cmake_minimum_required(VERSION 3.9)
include_directories(.)
set(LINKER_SCRIPT "link.T")

add_library(gb17_libretro SHARED
            libretro.cpp)

set_target_properties(gb17_libretro PROPERTIES PREFIX "")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")
