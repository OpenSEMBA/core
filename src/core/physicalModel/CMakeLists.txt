cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(opensemba_core_physicalmodel CXX)

add_library(opensemba_core_physicalmodel
    "PhysicalModel.cpp"
    "PriorityMaterial.cpp"
    "Bound.cpp"
    "multiport/Multiport.cpp"
    "multiport/Predefined.cpp"
    "multiport/Dispersive.cpp"
    "multiport/RLC.cpp"
    "surface/Multilayer.cpp"
    "surface/SIBC.cpp"
    "surface/SIBCFile.cpp"
    "wire/Extremes.cpp"
    "wire/Wire.cpp"
)
target_link_libraries(opensemba_core_physicalmodel opensemba_core_math
                                                   opensemba_core_filesystem)
