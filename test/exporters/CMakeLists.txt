cmake_minimum_required(VERSION 3.10)

message(STATUS "Configuring build system for opensemba_exporters_tests.")

find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include(GoogleTest)

add_executable(opensemba_exporters_tests 
    "ExporterVTKTest.cpp"
)

target_link_libraries(opensemba_exporters_tests 
    opensemba_exporters
    GTest::gtest     
    GTest::gtest_main
)


gtest_add_tests(
    TARGET opensemba_exporters_tests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
gtest_discover_tests(
    opensemba_exporters_tests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)