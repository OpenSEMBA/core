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

namespace SEMBA::parsers::JSON {

const std::string VERSION{ "0.16" };

using namespace Geometry;
using namespace math;

using json = nlohmann::json;
using PM = PhysicalModel::PhysicalModel;

class Parser : public SEMBA::parsers::Parser {
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

template<typename T>
Element::Group<ElemR> readElemStrAs(
        const PhysicalModel::Group<>& mG,
        const Layer::Group<>& lG,
        const CoordR3Group& cG,
        const json& e) {
    Element::Group<ElemR> res;

    for (auto it = e.begin(); it != e.end(); ++it) {

        ElemId elemId;
        MatId matId;
        Layer::Id layerId;
        std::vector<CoordId> vId;

        std::stringstream ss(it->get<std::string>());
        ss >> elemId >> matId >> layerId;
        vId.resize(T::sizeOfCoordinates);
        for (std::size_t j = 0; j < T::sizeOfCoordinates; j++) {
            ss >> vId[j];
        }

        const Layer::Layer* layerPtr;
        const PhysicalModel::PhysicalModel* matPtr;
        std::vector<const CoordR3*> vPtr;

        if (matId != MatId(0)) {
            matPtr = mG.getId(matId);
        } else {
            matPtr = nullptr;
        }
        if (layerId != LayerId(0)) {
            layerPtr = lG.getId(layerId);
        } else {
            layerPtr = nullptr;
        }
        vPtr.resize(vId.size(), nullptr);
        for (size_t i = 0; i < vId.size(); ++i) {
            vPtr[i] = cG.getId(vId[i]);
        }

        res.add(std::make_unique<T>(T(elemId, vPtr.data(), layerPtr, matPtr)));
    }

    return res;
}

} 