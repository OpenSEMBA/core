cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Configuring build system for opensemba tests.")

find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(opensemba_core_tests 
    "class/GroupIdentifiableUniqueTest.cpp"
    "filesystem/ProjectTest.cpp"
    "geometry/coordinate/CoordinateGroupTest.cpp"
    "geometry/element/Line2Test.cpp"
    "geometry/layer/LayerTest.cpp"
    "geometry/mesh/GeometricTest.cpp"
    "geometry/mesh/StructuredTest.cpp"
    "geometry/mesh/UnstructuredTest.cpp"
    "geometry/BoxTest.cpp"
    "geometry/GridTest.cpp"
    "math/axis/LocalTest.cpp"
    "math/function/GaussianTest.cpp"
    "math/function/LinearInterpolationTest.cpp"
    "math/matrix/DynamicTest.cpp"
    "math/matrix/StaticTest.cpp"
    "math/simplex/LineTest.cpp"
    "math/simplex/TetrahedronTest.cpp"
    "math/simplex/TriangleTest.cpp"
    "math/util/RealTest.cpp"
    "math/vector/CartesianVectorTest.cpp"
    "model/ModelTest.cpp"
    "physicalModel/multiport/RLCTest.cpp"
    "physicalModel/volume/ClassicTest.cpp"
    "physicalModel/wire/ExtremesTest.cpp"
    "physicalModel/wire/WireTest.cpp"
    "physicalModel/BoundTest.cpp"
    "physicalModel/GroupTest.cpp"
    "physicalModel/PriorityMaterialTest.cpp"
    "source/magnitude/NumericalTest.cpp"
    "source/port/TEMCoaxialTest.cpp"
    "source/port/WaveguideRectangularTest.cpp"
    "ProblemDescriptionTest.cpp"
)

target_link_libraries(opensemba_core_tests 
    opensemba_core    
    GTest::gtest 
)


