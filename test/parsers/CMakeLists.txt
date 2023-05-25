cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Configuring build system for opensemba_parsers_tests.")

find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(opensemba_parsers_tests 
    "json/ParserTest.cpp"
    "stl/ParserTest.cpp"
)

target_link_libraries(opensemba_parsers_tests 
    opensemba_parsers
    GTest::gtest     
    GTest::gtest_main
)

add_test(opensemba_parsers_tests ${CMAKE_BINARY_DIR}/bin/opensemba_parsers_tests)

include(GoogleTest)
gtest_discover_tests(opensemba_parsers_tests)