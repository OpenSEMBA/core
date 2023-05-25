cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Configuring build system for opensemba tests.")

add_subdirectory(core)
add_subdirectory(parsers)