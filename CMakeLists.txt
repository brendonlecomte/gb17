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

set(test_source "src/blaargh.cpp"
                 ${shared_sources})

add_executable(test-rom ${test_source})
add_test(NAME 01-Special     COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/01-special.gb")
# add_test(NAME 02-Interrupts  COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/02-interrupts.gb")
add_test(NAME 03-SP_HL       COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/03-op sp,hl.gb")
add_test(NAME 04-R_IMM       COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/04-op r,imm.gb")
add_test(NAME 05-RP          COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/05-op rp.gb")
add_test(NAME 06-LD_R_R      COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/06-ld r,r.gb")
# add_test(NAME 07-JR_JP       COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb")
# add_test(NAME 08-Misc        COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/08-misc instrs.gb")
# add_test(NAME 09-R_R         COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/09-op r,r.gb")
add_test(NAME 10-Bits        COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/10-bit ops.gb")
# add_test(NAME 11-A_HL        COMMAND test-rom  "${CMAKE_SOURCE_DIR}/gb-test-roms/cpu_instrs/individual/11-op a,(hl).gb")
