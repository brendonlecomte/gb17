cmake_minimum_required(VERSION 3.9)

project(gb17-test)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


# Generic Compile flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
set (core_sources "../src/CPU/CPU.cpp"
                  "../src/CPU/ALU.cpp"
                  "../src/CPU/Instructions.cpp"
                  "../src/MMU/MMU.cpp"
                  "../src/MMU/Cartridge.cpp"
                  "../src/MMU/Cartridge_Header.cpp"
                  "../src/Timer/Timer.cpp")

# Compile tests
add_executable(gb17_test_cpu "test_cpu.cpp" ${core_sources})
target_link_libraries(gb17_test_cpu gtest_main)
add_test(NAME Test_CPU COMMAND gb17_test_cpu)

add_executable(gb17_test_cart_header "test_cartridge_header.cpp" ${core_sources})
target_link_libraries(gb17_test_cart_header gtest_main)
add_test(NAME Test_Cart COMMAND gb17_test_cart_header)

add_executable(gb17_test_boot "test_boot.cpp" ${core_sources})
target_link_libraries(gb17_test_boot gtest_main)
add_test(NAME Test_Boot COMMAND gb17_test_boot)

add_executable(gb17_test_timer "test_timer.cpp" ${core_sources})
target_link_libraries(gb17_test_timer gtest_main)
add_test(NAME Test_Timer COMMAND gb17_test_timer)

add_executable(gb17_test_banking "test_banking.cpp" ${core_sources})
target_link_libraries(gb17_test_banking gtest_main)
add_test(NAME Test_Banks COMMAND gb17_test_banking)
