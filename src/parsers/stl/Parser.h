#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "core/geometry/mesh/Geometric.h"
#include "core/physicalModel/Predefined.h"

#include "parsers/Parser.h"

namespace SEMBA::parsers::STL {

class Parser : public SEMBA::parsers::Parser {
public:
    Parser(const std::string& fn);
    Geometry::Mesh::Unstructured readAsUnstructuredMesh() const;
};

}
