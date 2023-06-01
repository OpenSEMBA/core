#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <type_traits>

#include "core/geometry/mesh/Geometric.h"
#include "core/ProblemDescription.h"
#include "parsers/Parser.h"

namespace semba::parsers::JSON {

const std::string VERSION{ "0.16" };

using namespace geometry;
using namespace math;

using json = nlohmann::json;
using PM = PhysicalModel::PhysicalModel;

class Parser : public semba::parsers::Parser {
public:  
    Parser(const std::string& filename);
    UnstructuredProblemDescription read() const;
};

json readAnalysis(const json&);
std::unique_ptr<Mesh::Unstructured> readUnstructuredMesh(const PMGroup&, const json&, const std::string& folder);
Grid3 readGrids(const json&);
PMGroup readMaterials(const json&);
SourceGroup readSources(Mesh::Unstructured& mesh, const json&);
OutputRequestGroup readProbes(Mesh::Unstructured& mesh, const json&);
void readBoundary(Mesh::Unstructured& mesh, const json& j, PMGroup& physicalModelGroup, const Grid3& grid);


} 