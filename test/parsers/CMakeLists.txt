cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Configuring build system for opensemba tests.")

find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(opensemba_parsers_tests 
    "json/ParserTest.cpp"
    "stl/ParserTest.cpp"
)

target_link_libraries(opensemba_parsers_tests 
    opensemba_parsers
    GTest::gtest     
)


