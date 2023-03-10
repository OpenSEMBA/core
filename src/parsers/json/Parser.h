#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <type_traits>

#include "geometry/mesh/Geometric.h"
#include "physicalModel/Bound.h"
#include "physicalModel/multiport/Multiport.h"
#include "physicalModel/surface/Multilayer.h"
#include "physicalModel/surface/SIBCFile.h"
#include "physicalModel/volume/Dispersive.h"
#include "physicalModel/volume/Anisotropic.h"

#include "source/Dipole.h"
#include "source/Generator.h"
#include "source/OnLine.h"
#include "source/PlaneWave.h"
#include "source/port/Waveguide.h"
#include "source/port/TEM.h"

#include "ProblemDescription.h"

#include "parsers/Parser.h"

namespace SEMBA::parsers::JSON {

const std::string VERSION{ "0.16" };

using namespace Geometry;
using namespace Math;

using json = nlohmann::json;
using PM = PhysicalModel::PhysicalModel;

class Parser : public SEMBA::parsers::Parser {
public:  
    Parser(const std::string& filename);
    UnstructuredProblemDescription read() const;
    
private:
    json readSolverOptions(const json&, const std::string& key = "solverOptions") const;

	void readConnectorOnPoint(PMGroup& pMG, Mesh::Unstructured& mesh,  const json&) const;
    
    Grid3 readGrids(const json&) const;
    Grid3 buildGridFromFile(const FileSystem::Project& file) const;

    std::unique_ptr<Mesh::Unstructured> readUnstructuredMesh(const PMGroup&, const json&) const;
    LayerGroup readLayers(const json&) const;    
    CoordR3Group readCoordinates(const json&) const;
    ElemRGroup readElements(const PMGroup&, LayerGroup&, CoordR3Group&, const json&) const;
    ElemRGroup readElementsFromFile(const PMGroup&, LayerGroup&, CoordR3Group&, const json&) const;
    ElemRGroup readElementsFromSTLFile(const PMGroup&, LayerGroup&, CoordR3Group&, const json&) const;


};

CVecI3 strToCVecI3(std::string str);
CVecR3 strToCVecR3(std::string str);
Constants::CartesianAxis strToCartesianAxis(std::string);
std::pair<CVecR3, CVecR3> strToBox(const std::string& str);
Axis::Local strToLocalAxes(const std::string& str);

const ElemR* boxToElemGroup(Mesh::Unstructured& mesh, const std::string& line);

PMGroup readPhysicalModels(const json&);
std::unique_ptr<PhysicalModel::Surface::Multilayer> readMultilayerSurface(const json& layers);
std::unique_ptr<PM> readPhysicalModel(const json& material);
PhysicalModel::PhysicalModel::Type strToMaterialType(std::string label);
PhysicalModel::Multiport::Multiport::Type strToMultiportType(std::string label);
PhysicalModel::Volume::Anisotropic::Model strToAnisotropicModel(std::string label);

SourceGroup readSources(Mesh::Unstructured& mesh, const json&);
std::unique_ptr<Source::PlaneWave> readPlanewave(Mesh::Unstructured& mesh, const json&);
std::unique_ptr<Source::Port::Waveguide> readPortWaveguide(Mesh::Unstructured& mesh, const json&);
std::unique_ptr<Source::Port::TEM> readPortTEM(Mesh::Unstructured& mesh, const json&);
std::unique_ptr<Source::Generator> readGenerator(Mesh::Unstructured& mesh, const json&);
std::unique_ptr<Source::OnLine> readSourceOnLine(Mesh::Unstructured& mesh, const json&);
std::unique_ptr<Source::Magnitude::Magnitude> readMagnitude(const json&);
Source::Generator::Type strToGeneratorType(std::string label);
Source::Generator::Hardness strToGeneratorHardness(std::string str);
Source::OnLine::Type strToNodalType(std::string label);
Source::OnLine::Hardness strToNodalHardness(std::string label);
Source::Port::TEM::ExcitationMode strToTEMMode(std::string);
Source::Port::Waveguide::ExcitationMode strToWaveguideMode(std::string);

OutputRequestGroup readOutputRequests(Mesh::Unstructured& mesh, const json&);
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