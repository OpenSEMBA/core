cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Configuring build system for opensemba tests.")

add_executable(opensemba_tests "null.cpp")

add_subdirectory(core)
add_subdirectory(parsers)

find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(opensemba_tests
    opensemba_core_tests
    opensemba_parsers_tests
    GTest::gtest_main
)

add_test(opensemba  ${CMAKE_BINARY_DIR}/bin/opensemba_tests)


