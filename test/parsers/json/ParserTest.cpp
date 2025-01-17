#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>


#include "parsers/json/Parser.h"

#include "core/math/function/Gaussian.h"
#include "core/geometry/element/Line2.h"
#include "core/geometry/element/Polyline.h"
#include "core/geometry/element/Triangle3.h"
#include "core/geometry/element/Tetrahedron4.h"
#include "core/outputRequest/FarField.h"
#include "core/outputRequest/OnPoint.h"
#include "core/physicalModel/multiport/MultiWirePort.h"
#include "core/physicalModel/wire/MultiWire.h"
#include "core/physicalModel/wire/Wire.h"
#include "core/physicalModel/multiport/RLC.h"
#include "core/physicalModel/Predefined.h"
#include "core/physicalModel/volume/Classic.h"
#include "core/source/PlaneWave.h"
#include "core/source/Generator.h"

using namespace semba;
using namespace semba::parsers::JSON;
using namespace geometry::mesh;

class ParserJSONParserTest : public ::testing::Test {
protected:
    static std::string getFolder() {
        return "testData/";
    }

    static std::string getFilename(const std::string& name)
    {
        return getFolder() + name + "/" + name + ".smb.json";
    }
};

TEST_F(ParserJSONParserTest, dmcwf) 
{
    ASSERT_NO_THROW(Parser{ getFilename("dmcwf") }.read());
}

TEST_F(ParserJSONParserTest, sphere)
{
    ASSERT_NO_THROW(Parser{ getFilename("sphere") }.read());
}

TEST_F(ParserJSONParserTest, antennas)
{
    ASSERT_NO_THROW(Parser{ getFilename("antennas") }.read());
}

TEST_F(ParserJSONParserTest, wires)
{
    ASSERT_NO_THROW(Parser{ getFilename("wires") }.read());
}

TEST_F(ParserJSONParserTest, bowtie)
{
    ASSERT_NO_THROW(Parser{ getFilename("bowtie") }.read());
}

TEST_F(ParserJSONParserTest, b2)
{
    ASSERT_NO_THROW(Parser{ getFilename("b2") }.read());
}

TEST_F(ParserJSONParserTest, wire_probes)
{
    ASSERT_NO_THROW(Parser{ getFilename("wire_probes") }.read());
}

TEST_F(ParserJSONParserTest, b2_detailed)
{
    auto data{ Parser{getFilename("b2") }.read() };

    EXPECT_EQ(353, data.model.mesh.coords().size());
    EXPECT_EQ(652, data.model.mesh.elems().sizeOf<geometry::Tri3>());
}

TEST_F(ParserJSONParserTest, bowtie_detailed)
{
    auto data{ Parser(getFilename("bowtie")).read() };

    EXPECT_EQ(422, data.model.mesh.coords().size());
    EXPECT_EQ(836, data.model.mesh.elems().size());
}

TEST_F(ParserJSONParserTest, sphere_detailed) 
{
    auto data{ Parser(getFilename("sphere")).read() };

    EXPECT_EQ(data.grids.getNumCells(), math::CVecR3(51, 23, 15));

    auto& sources = data.sources;
    EXPECT_EQ(sources.size(), 1);

    const source::PlaneWave* source = sources.get()[0]->castTo<source::PlaneWave>();
    EXPECT_EQ(
        source->getPolarization(),
        math::CVecR3(-0.4082482904638631, 0.8164965809277261, -0.4082482904638631)
    );
    EXPECT_EQ(
        source->getDirection(),
        math::CVecR3(1.0, 1.0, 1.0)
    );

    source::Magnitude::Magnitude magnitude = *source->getMagnitude();
    EXPECT_EQ(
        magnitude,
        source::Magnitude::Magnitude(
            new math::function::Gaussian(
                math::function::Gaussian::buildFromMaximumFrequency(
                    1000000000.0,
                    1.0
                )
            )
        )
    );

    auto& model = data.model;

    // 2 predefined + 6 bounds
    EXPECT_EQ(8, model.physicalModels.size());
    EXPECT_EQ("pec", model.physicalModels.get()[0]->getName());
    EXPECT_EQ("pmc", model.physicalModels.get()[1]->getName());

    const physicalModel::Bound::Type boundaryLowerUpperMaterials[6] = {
        physicalModel::Bound::Type::pml,
        physicalModel::Bound::Type::pec,
        physicalModel::Bound::Type::pmc,
        physicalModel::Bound::Type::mur1,
        physicalModel::Bound::Type::mur2,
        physicalModel::Bound::Type::periodic
    };

    auto boundaries = model.physicalModels.getOf<physicalModel::Bound>();
    for (auto& bound : boundaryLowerUpperMaterials) {
        EXPECT_TRUE(
            std::find_if(
                boundaries.cbegin(),
                boundaries.cend(),
                [&](const physicalModel::Bound* boundI) -> bool {return boundI->getType() == bound; }
            ) != boundaries.cend()
        );
    }

    // 384 coming from grid, 8 from source, 1 from point probe and 8 from far field
    // New elements added as part of Boundaries: 6 faces * 4 points/face
    ASSERT_EQ(384 + 8 + 1 + 8 + 24, model.mesh.coords().size());
    EXPECT_EQ(
        math::CVecR3(2.33333325, -5.71501865e-16, 1.66666663),
        model.mesh.coords().get()[0]->pos()
    );
    EXPECT_EQ(
        math::CVecR3(1.28204191, -1.31762123e+01, -1.70370862e-01),
        model.mesh.coords().get()[383]->pos()
    );

    auto& probes = data.outputRequests;

    ASSERT_EQ(2, probes.size());
    EXPECT_EQ(outputRequest::OutputRequest::Type::electric, probes.get()[0]->getType());
    EXPECT_EQ(outputRequest::OutputRequest::Type::electricFarField, probes.get()[1]->getType());

    ASSERT_EQ(1, probes.get()[0]->getTarget().size());

    auto recoveredNodeId = probes.get()[0]->getTarget().at(0);
    auto recoveredNode = model.mesh.elems().atId(recoveredNodeId)->castTo<geometry::NodR>();
    EXPECT_EQ(
        math::CVecR3(-0.8441360141053171, 12.017228978451016, 13.154724231963254),
        recoveredNode->getV(0)->pos()
    );
}

TEST_F(ParserJSONParserTest, sphere_rectilinear) 
{
    auto data{ Parser(getFolder() + "sphere/sphere-rectilinear.smb.json").read() };

    EXPECT_EQ(math::CVecI3(1,2,3), data.grids.getNumCells());
    EXPECT_EQ(std::vector<math::Real>({0, 1}), data.grids.getPos(0));
    EXPECT_EQ(std::vector<math::Real>({0, 1, 2}), data.grids.getPos(1));
    EXPECT_EQ(std::vector<math::Real>({0, 1, 2, 3}), data.grids.getPos(2));
}

TEST_F(ParserJSONParserTest, sphere_onePlaneFarField)
{
    auto data{ Parser{ getFolder() + "sphere/sphere-one-plane-farfield.smb.json" }.read() };

    EXPECT_EQ(data.outputRequests.sizeOf<outputRequest::FarField>(), 1);
    auto farFieldProbe{ data.outputRequests.getOf<outputRequest::FarField>().front() };
    EXPECT_EQ(farFieldProbe->initialPhi, 0.0);
    EXPECT_EQ(farFieldProbe->finalPhi, 0.0);
    EXPECT_EQ(farFieldProbe->stepPhi, 0.1 * 2 * math::Constants::pi / 360.0);
}

TEST_F(ParserJSONParserTest, antennas_detailed)
{
    auto data{ Parser{ getFilename("antennas") }.read() };

    EXPECT_EQ(data.outputRequests.sizeOf<outputRequest::OnPoint>(), 3);
    EXPECT_EQ(data.sources.sizeOf<source::Generator>(), 1);
    EXPECT_EQ(data.model.mesh.elems().sizeOf<geometry::NodR>(), 5);

    EXPECT_EQ(data.model.physicalModels.size(), 5); // Cable, 2 connector, 2 bounds (pec and pml)

    auto materialCableList = data.model.physicalModels.getOf<physicalModel::wire::Wire>();
    EXPECT_EQ(materialCableList.size(), 1);

    auto materialPortList = data.model.physicalModels.getOf<physicalModel::multiport::RLC>();
    EXPECT_EQ(materialPortList.size(), 1);

    auto& materialCable = materialCableList.front();
    auto& materialPort = materialPortList.front();

    geometry::ElemView elementsWithCableMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialCable->getId()) {
            elementsWithCableMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithCableMaterial.size(), 2);

    geometry::ElemView elementsWithPortMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialPort->getId()) {
            elementsWithPortMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithPortMaterial.size(), 2);
}

TEST_F(ParserJSONParserTest, twoWiresDetailed)
{
    std::ifstream stream(getFolder() + "/wires/two_wires.smb.json");
    json j;
    stream >> j;
    auto& modelJson = j.at("model");

    auto physicalModel{ parsers::JSON::readMaterials(modelJson)};

    auto& unstructuredMesh = parsers::JSON::readUnstructuredMesh(physicalModel, modelJson, getFolder() + "/wires");
    auto& mesh = *(unstructuredMesh.get());
    auto& bundles = parsers::JSON::readBundles(physicalModel, mesh.elems(), modelJson);

    EXPECT_EQ(3, physicalModel.size());
    EXPECT_EQ(1, mesh.layers().size());
    EXPECT_EQ(6, mesh.coords().size());
    EXPECT_EQ(2, mesh.elems().size());
    EXPECT_EQ(1, mesh.junctions().size());
    ASSERT_EQ(2, bundles.size());

    auto& wireMaterialList = physicalModel.getOf<physicalModel::wire::Wire>();

    ASSERT_EQ(1, wireMaterialList.size());

    auto& connectorMaterialList = physicalModel.getOf<physicalModel::multiport::Multiport>();

    ASSERT_EQ(2, connectorMaterialList.size());

    auto& polylinesList = mesh.elems().getOf<PolylinR>();

    ASSERT_EQ(2, polylinesList.size());

    auto& firstPolyline = polylinesList[0];
    auto& secondPolyline = polylinesList[1];

    EXPECT_EQ(MatId(0), firstPolyline->getMatId());
    EXPECT_EQ(MatId(0), secondPolyline->getMatId());

    auto layer = mesh.layers().atId(geometry::LayerId(1));
    EXPECT_EQ(layer, firstPolyline->getLayer());
    EXPECT_EQ(layer, secondPolyline->getLayer());

    auto& firstCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(1),geometry::CoordId(2), geometry::CoordId(3) });
    auto& secondCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(4),geometry::CoordId(5), geometry::CoordId(6) });

    EXPECT_TRUE(element::Base::areSameCoords(firstCoordinatesSubList, firstPolyline->getCoordinates()));
    EXPECT_TRUE(element::Base::areSameCoords(secondCoordinatesSubList, secondPolyline->getCoordinates()));

    EXPECT_THAT("j1", ::testing::StrEq(mesh.junctions()[0].getName()));

    const auto & unitedCoordIds = mesh.junctions()[0].getUnitedCoordIds();

    ASSERT_EQ(1, unitedCoordIds.size());
    EXPECT_THAT(unitedCoordIds[0], ::testing::ElementsAre(
        firstPolyline->getVertices().back()->getId(),
        secondPolyline->getVertices().front()->getId()
    ));
    auto firstCoordinate = mesh.coords().atId(unitedCoordIds[0][0]);
    auto secondCoordinate = mesh.coords().atId(unitedCoordIds[0][1]);

    EXPECT_EQ(firstCoordinate->pos(), secondCoordinate->pos());

    auto& firstBundle = bundles[0];
    auto& secondBundle = bundles[1];

    EXPECT_THAT("wire1", ::testing::StrEq(firstBundle.getName()));
    EXPECT_THAT("wire2", ::testing::StrEq(secondBundle.getName()));
    EXPECT_EQ(wireMaterialList[0]->getId(), firstBundle.getMaterialId());
    EXPECT_EQ(wireMaterialList[0]->getId(), secondBundle.getMaterialId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), firstBundle.getInitialConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), firstBundle.getEndConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), secondBundle.getInitialConnectorId());
    EXPECT_EQ(connectorMaterialList[0]->getId(), secondBundle.getEndConnectorId());
    EXPECT_THAT(firstBundle.getElemIds(), ::testing::ElementsAre(firstPolyline->getId()));
    EXPECT_THAT(secondBundle.getElemIds(), ::testing::ElementsAre(secondPolyline->getId()));
}

TEST_F(ParserJSONParserTest, threeWiresDetailed)
{
    std::ifstream stream(getFolder() + "/wires/three_wires.smb.json");
    json j;
    stream >> j;
    auto& modelJson = j.at("model");

    auto physicalModel{ parsers::JSON::readMaterials(modelJson) };

    auto& unstructuredMesh = parsers::JSON::readUnstructuredMesh(physicalModel, modelJson, getFolder() + "/wires");
    auto& mesh = *(unstructuredMesh.get());
    auto& bundles = parsers::JSON::readBundles(physicalModel, mesh.elems(), modelJson);

    EXPECT_EQ(3, physicalModel.size());
    EXPECT_EQ(1, mesh.layers().size());
    EXPECT_EQ(8, mesh.coords().size());
    EXPECT_EQ(3, mesh.elems().size());
    EXPECT_EQ(1, mesh.junctions().size());
    ASSERT_EQ(3, bundles.size());

    auto& wireMaterialList = physicalModel.getOf<physicalModel::wire::Wire>();

    ASSERT_EQ(1, wireMaterialList.size());

    auto& connectorMaterialList = physicalModel.getOf<physicalModel::multiport::Multiport>();

    ASSERT_EQ(2, connectorMaterialList.size());

    auto& polylinesList = mesh.elems().getOf<PolylinR>();

    ASSERT_EQ(3, polylinesList.size());

    auto& firstPolyline = polylinesList[0];
    auto& secondPolyline = polylinesList[1];
    auto& thirdPolyline = polylinesList[2];

    EXPECT_EQ(MatId(0), firstPolyline->getMatId());
    EXPECT_EQ(MatId(0), secondPolyline->getMatId());
    EXPECT_EQ(MatId(0), thirdPolyline->getMatId());

    auto layer = mesh.layers().atId(geometry::LayerId(1));
    EXPECT_EQ(layer, firstPolyline->getLayer());
    EXPECT_EQ(layer, secondPolyline->getLayer());
    EXPECT_EQ(layer, thirdPolyline->getLayer());

    auto& firstCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(1),geometry::CoordId(2), geometry::CoordId(3) });
    auto& secondCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(4),geometry::CoordId(5), geometry::CoordId(6) });
    auto& thirdCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(7),geometry::CoordId(8)});

    EXPECT_TRUE(element::Base::areSameCoords(firstCoordinatesSubList, firstPolyline->getCoordinates()));
    EXPECT_TRUE(element::Base::areSameCoords(secondCoordinatesSubList, secondPolyline->getCoordinates()));
    EXPECT_TRUE(element::Base::areSameCoords(thirdCoordinatesSubList, thirdPolyline->getCoordinates()));

    EXPECT_THAT("j1", ::testing::StrEq(mesh.junctions()[0].getName()));

    const auto& unitedCoordIds = mesh.junctions()[0].getUnitedCoordIds();

    ASSERT_EQ(1, unitedCoordIds.size());
    EXPECT_THAT(unitedCoordIds[0], ::testing::ElementsAre(
        firstPolyline->getVertices().back()->getId(),
        secondPolyline->getVertices().front()->getId(),
        thirdPolyline->getVertices().front()->getId()
    ));
    auto firstCoordinate = mesh.coords().atId(unitedCoordIds[0][0]);
    auto secondCoordinate = mesh.coords().atId(unitedCoordIds[0][1]);
    auto thirdCoordinate = mesh.coords().atId(unitedCoordIds[0][2]);

    EXPECT_EQ(firstCoordinate->pos(), secondCoordinate->pos());
    EXPECT_EQ(firstCoordinate->pos(), thirdCoordinate->pos());

    auto& firstBundle = bundles[0];
    auto& secondBundle = bundles[1];
    auto& thirdBundle = bundles[2];

    EXPECT_THAT("wire1", ::testing::StrEq(firstBundle.getName()));
    EXPECT_THAT("wire2", ::testing::StrEq(secondBundle.getName()));
    EXPECT_THAT("wire3", ::testing::StrEq(thirdBundle.getName()));
    EXPECT_EQ(wireMaterialList[0]->getId(), firstBundle.getMaterialId());
    EXPECT_EQ(wireMaterialList[0]->getId(), secondBundle.getMaterialId());
    EXPECT_EQ(wireMaterialList[0]->getId(), thirdBundle.getMaterialId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), firstBundle.getInitialConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), firstBundle.getEndConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), secondBundle.getInitialConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), secondBundle.getEndConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), thirdBundle.getInitialConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), thirdBundle.getEndConnectorId());
    EXPECT_THAT(firstBundle.getElemIds(), ::testing::ElementsAre(firstPolyline->getId()));
    EXPECT_THAT(secondBundle.getElemIds(), ::testing::ElementsAre(secondPolyline->getId()));
    EXPECT_THAT(thirdBundle.getElemIds(), ::testing::ElementsAre(thirdPolyline->getId()));
}

TEST_F(ParserJSONParserTest, bundleAndTwoWiresDetailed)
{
    std::ifstream stream(getFolder() + "/wires/cable_bundle_and_two_wires.smb.json");
    json j;
    stream >> j;
    auto& modelJson = j.at("model");

    auto physicalModel{ parsers::JSON::readMaterials(modelJson) };

    auto& unstructuredMesh = parsers::JSON::readUnstructuredMesh(physicalModel, modelJson, getFolder() + "/wires");
    auto& mesh = *(unstructuredMesh.get());
    auto& bundles = parsers::JSON::readBundles(physicalModel, mesh.elems(), modelJson);

    EXPECT_EQ(5, physicalModel.size());
    EXPECT_EQ(1, mesh.layers().size());
    EXPECT_EQ(8, mesh.coords().size());
    EXPECT_EQ(4, mesh.elems().size());
    EXPECT_EQ(1, mesh.junctions().size());
    ASSERT_EQ(3, bundles.size());

    auto& wireMaterialList = physicalModel.getOf<physicalModel::wire::Wire>();

    ASSERT_EQ(1, wireMaterialList.size());

    auto& connectorMaterialList = physicalModel.getOf<physicalModel::multiport::Multiport>();

    ASSERT_EQ(3, connectorMaterialList.size());
    ASSERT_EQ(1, physicalModel.sizeOf<physicalModel::multiport::MultiWirePort>());


    auto& multiWireMaterialList = physicalModel.getOf<physicalModel::wire::MultiWire>();

    ASSERT_EQ(1, multiWireMaterialList.size());

    auto& linesList = mesh.elems().getOf<LinR2>();

    ASSERT_EQ(4, linesList.size());

    auto& firstLine = linesList[0];
    auto& secondLine = linesList[1];
    auto& thirdLine = linesList[2];
    auto& fourthLine = linesList[3];

    EXPECT_EQ(MatId(0), firstLine->getMatId());
    EXPECT_EQ(MatId(0), secondLine->getMatId());
    EXPECT_EQ(MatId(0), thirdLine->getMatId());
    EXPECT_EQ(MatId(0), fourthLine->getMatId());

    auto layer = mesh.layers().atId(geometry::LayerId(1));
    EXPECT_EQ(layer, firstLine->getLayer());
    EXPECT_EQ(layer, secondLine->getLayer());
    EXPECT_EQ(layer, thirdLine->getLayer());
    EXPECT_EQ(layer, fourthLine->getLayer());

    auto& firstCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(1),geometry::CoordId(2) });
    auto& secondCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(3), geometry::CoordId(4) });
    auto& thirdCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(5), geometry::CoordId(6) });
    auto& fourthCoordinatesSubList = mesh.coords().atIds({ geometry::CoordId(7),geometry::CoordId(8) });

    EXPECT_TRUE(element::Base::areSameCoords(firstCoordinatesSubList, firstLine->getCoordinates()));
    EXPECT_TRUE(element::Base::areSameCoords(secondCoordinatesSubList, secondLine->getCoordinates()));
    EXPECT_TRUE(element::Base::areSameCoords(thirdCoordinatesSubList, thirdLine->getCoordinates()));
    EXPECT_TRUE(element::Base::areSameCoords(fourthCoordinatesSubList, fourthLine->getCoordinates()));

    EXPECT_THAT("j1", ::testing::StrEq(mesh.junctions()[0].getName()));

    const auto& unitedCoordIds = mesh.junctions()[0].getUnitedCoordIds();

    ASSERT_EQ(2, unitedCoordIds.size());
    EXPECT_THAT(unitedCoordIds[0], ::testing::ElementsAre(
        firstLine->getVertices().back()->getId(),
        thirdLine->getVertices().front()->getId()
    ));
    EXPECT_THAT(unitedCoordIds[1], ::testing::ElementsAre(
        secondLine->getVertices().back()->getId(),
        fourthLine->getVertices().front()->getId()
    ));
    auto firstCoordinate = mesh.coords().atId(unitedCoordIds[0][0]);
    auto secondCoordinate = mesh.coords().atId(unitedCoordIds[0][1]);
    auto thirdCoordinate = mesh.coords().atId(unitedCoordIds[1][0]);
    auto fourthCoordinate = mesh.coords().atId(unitedCoordIds[1][1]);

    EXPECT_EQ(firstCoordinate->pos(), secondCoordinate->pos());
    EXPECT_EQ(firstCoordinate->pos(), thirdCoordinate->pos());
    EXPECT_EQ(firstCoordinate->pos(), fourthCoordinate->pos());

    auto& bundle = bundles[0];
    auto& firstCable = bundles[1];
    auto& secondCable = bundles[2];

    EXPECT_THAT("bundle1", ::testing::StrEq(bundle.getName()));
    EXPECT_THAT("wire2", ::testing::StrEq(firstCable.getName()));
    EXPECT_THAT("wire3", ::testing::StrEq(secondCable.getName()));
    EXPECT_EQ(multiWireMaterialList[0]->getId(), bundle.getMaterialId());
    EXPECT_EQ(wireMaterialList[0]->getId(), firstCable.getMaterialId());
    EXPECT_EQ(wireMaterialList[0]->getId(), secondCable.getMaterialId());
    EXPECT_EQ(connectorMaterialList[2]->getId(), bundle.getInitialConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), bundle.getEndConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), firstCable.getInitialConnectorId());
    EXPECT_EQ(connectorMaterialList[0]->getId(), firstCable.getEndConnectorId());
    EXPECT_EQ(connectorMaterialList[1]->getId(), secondCable.getInitialConnectorId());
    EXPECT_EQ(connectorMaterialList[0]->getId(), secondCable.getEndConnectorId());
    EXPECT_THAT(bundle.getElemIds(), ::testing::ElementsAre(firstLine->getId(), secondLine->getId()));
    EXPECT_THAT(firstCable.getElemIds(), ::testing::ElementsAre(thirdLine->getId()));
    EXPECT_THAT(secondCable.getElemIds(), ::testing::ElementsAre(fourthLine->getId()));
}


TEST_F(ParserJSONParserTest, readMaterials)
{
    std::ifstream stream(getFolder() + "materials.smb.json");
    json j;
    stream >> j;

    auto materials{ parsers::JSON::readMaterials(j) };

    EXPECT_EQ(6, materials.size());
    EXPECT_EQ(1, materials.sizeOf<physicalModel::PEC>());
    EXPECT_EQ(1, materials.sizeOf<physicalModel::PMC>());
    EXPECT_EQ(1, materials.sizeOf<physicalModel::SMA>());
    ASSERT_EQ(1, materials.sizeOf<physicalModel::volume::Classic>());
    ASSERT_EQ(1, materials.sizeOf<physicalModel::wire::MultiWire>());
    ASSERT_EQ(1, materials.sizeOf<physicalModel::multiport::Multiport>());

    auto classic = materials.getOf<physicalModel::volume::Classic>()[0];
    EXPECT_DOUBLE_EQ(1.0, classic->getRelativePermittivity());
    EXPECT_DOUBLE_EQ(2.0, classic->getRelativePermeability());
    EXPECT_DOUBLE_EQ(3.0, classic->getElectricConductivity());
    EXPECT_DOUBLE_EQ(4.0, classic->getMagneticConductivity());

    auto multiWire = materials.getOf<physicalModel::wire::MultiWire>()[0];
    EXPECT_THAT(multiWire->getResistanceVector(), ::testing::ElementsAre(1.0, 2.0));
    EXPECT_THAT(multiWire->getInductanceMatrix(), ::testing::ElementsAre(
        std::vector<math::Real>{3.0, 4.0},
        std::vector<math::Real>{5.0, 6.0}
    ));
    EXPECT_THAT(multiWire->getCapacitanceMatrix(), ::testing::ElementsAre(
        std::vector<math::Real>{7.0, 8.0},
        std::vector<math::Real>{9.0, 10.0}
    ));

    auto multiWireConnector = materials.getOf<physicalModel::multiport::MultiWirePort>()[0];
    EXPECT_THAT(multiWireConnector->getResistanceVector(), ::testing::ElementsAre(1.0, 2.0));
}