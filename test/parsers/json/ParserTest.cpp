
#include "gtest/gtest.h"

#include "parsers/json/Parser.h"

#include "geometry/element/Line2.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Tetrahedron4.h"
#include "math/function/Gaussian.h"
#include "outputRequest/FarField.h"
#include "outputRequest/OnPoint.h"
#include "physicalModel/wire/Wire.h"
#include "physicalModel/multiport/RLC.h"

using namespace SEMBA;
using namespace SEMBA::Parsers::JSON;
using namespace Geometry::Mesh;

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

TEST_F(ParserJSONParserTest, b2_detailed)
{
    auto data{ Parser{getFilename("b2") }.read() };

    EXPECT_EQ(353, data.model.mesh.coords().size());
    EXPECT_EQ(652, data.model.mesh.elems().sizeOf<Geometry::Tri3>());
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

    EXPECT_EQ(data.grids.getNumCells(), Math::CVecR3(51, 23, 15));

    auto& sources = data.sources;
    EXPECT_EQ(sources.size(), 1);

    const Source::PlaneWave* source = sources.get()[0]->castTo<Source::PlaneWave>();
    EXPECT_EQ(
        source->getPolarization(),
        Math::CVecR3(-0.4082482904638631, 0.8164965809277261, -0.4082482904638631)
    );
    EXPECT_EQ(
        source->getDirection(),
        Math::CVecR3(1.0, 1.0, 1.0)
    );

    Source::Magnitude::Magnitude magnitude = *source->getMagnitude();
    EXPECT_EQ(
        magnitude,
        Source::Magnitude::Magnitude(
            new Math::Function::Gaussian(
                Math::Function::Gaussian::buildFromMaximumFrequency(
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

    const PhysicalModel::Bound::Type boundaryLowerUpperMaterials[6] = {
        PhysicalModel::Bound::Type::pml,
        PhysicalModel::Bound::Type::pec,
        PhysicalModel::Bound::Type::pmc,
        PhysicalModel::Bound::Type::mur1,
        PhysicalModel::Bound::Type::mur2,
        PhysicalModel::Bound::Type::periodic
    };

    auto& boundaries = model.physicalModels.getOf<PhysicalModel::Bound>();
    for (auto& bound : boundaryLowerUpperMaterials) {
        EXPECT_TRUE(
            std::find_if(
                boundaries.cbegin(),
                boundaries.cend(),
                [&](const PhysicalModel::Bound* boundI) -> bool {return boundI->getType() == bound; }
            ) != boundaries.cend()
        );
    }

    // 384 coming from grid, 8 from source, 1 from point probe and 8 from far field
    // New elements added as part of Boundaries: 6 faces * 4 points/face
    ASSERT_EQ(384 + 8 + 1 + 8 + 24, model.mesh.coords().size());
    EXPECT_EQ(
        Math::CVecR3(2.33333325, -5.71501865e-16, 1.66666663),
        model.mesh.coords().get()[0]->pos()
    );
    EXPECT_EQ(
        Math::CVecR3(1.28204191, -1.31762123e+01, -1.70370862e-01),
        model.mesh.coords().get()[383]->pos()
    );

    auto& probes = data.outputRequests;

    ASSERT_EQ(2, probes.size());
    EXPECT_EQ(OutputRequest::OutputRequest::Type::electric, probes.get()[0]->getType());
    EXPECT_EQ(OutputRequest::OutputRequest::Type::electricFarField, probes.get()[1]->getType());

    ASSERT_EQ(1, probes.get()[0]->getTarget().size());

    auto recoveredNode = probes.get()[0]->getTarget().at(0)->castTo<Geometry::NodR>();
    EXPECT_EQ(
        Math::CVecR3(-0.8441360141053171, 12.017228978451016, 13.154724231963254),
        recoveredNode->getV(0)->pos()
    );
}

TEST_F(ParserJSONParserTest, sphere_rectilinear) 
{
    auto data{ 
        Parser(getFolder() + "sphere/sphere-rectilinear.smb.json").read() 
    };

    EXPECT_EQ(Math::CVecI3(1,2,3), data.grids.getNumCells());
    EXPECT_EQ(std::vector<Math::Real>({0, 1}), data.grids.getPos(0));
    EXPECT_EQ(std::vector<Math::Real>({0, 1, 2}), data.grids.getPos(1));
    EXPECT_EQ(std::vector<Math::Real>({0, 1, 2, 3}), data.grids.getPos(2));
}

TEST_F(ParserJSONParserTest, sphere_onePlaneFarField)
{
    auto data{
        Parser{ getFolder() + "sphere/sphere-one-plane-farfield.smb.json" }.read()
    };

    EXPECT_EQ(data.outputRequests.sizeOf<OutputRequest::FarField>(), 1);
    auto farFieldProbe{ data.outputRequests.getOf<OutputRequest::FarField>().front() };
    EXPECT_EQ(farFieldProbe->initialPhi, 0.0);
    EXPECT_EQ(farFieldProbe->finalPhi, 0.0);
    EXPECT_EQ(farFieldProbe->stepPhi, 0.1 * 2 * Math::Constants::pi / 360.0);
}

TEST_F(ParserJSONParserTest, antennas_detailed)
{
    auto data{ Parser{ getFilename("antennas") }.read() };

    EXPECT_EQ(data.outputRequests.sizeOf<OutputRequest::OnPoint>(), 3);
    EXPECT_EQ(data.sources.sizeOf<Source::Generator>(), 1);
    EXPECT_EQ(data.model.mesh.elems().sizeOf<Geometry::NodR>(), 6);

    EXPECT_EQ(data.model.physicalModels.size(), 5); // Cable, 2 connector, 2 bounds (pec and pml)

    auto& materialCableList = data.model.physicalModels.getOf<PhysicalModel::Wire::Wire>();
    EXPECT_EQ(materialCableList.size(), 1);

    auto& materialPortList = data.model.physicalModels.getOf<PhysicalModel::Multiport::RLC>();
    EXPECT_EQ(materialPortList.size(), 1);

    auto& materialCable = materialCableList.front();
    auto& materialPort = materialPortList.front();

    Geometry::ElemView elementsWithCableMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialCable->getId()) {
            elementsWithCableMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithCableMaterial.size(), 2);

    Geometry::ElemView elementsWithPortMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialPort->getId()) {
            elementsWithPortMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithPortMaterial.size(), 2);
}

TEST_F(ParserJSONParserTest, antennas_probesUsingCoordIds)
{
    auto data{ 
        Parser{getFolder() + "antennas/antennas-probes-with-coordIds.smb.json"}.read()
    };

    EXPECT_EQ(data.outputRequests.sizeOf<OutputRequest::OnPoint>(), 3);
    EXPECT_EQ(data.sources.sizeOf<Source::Generator>(), 1);
    EXPECT_EQ(data.model.mesh.elems().sizeOf<Geometry::NodR>(), 9);

    EXPECT_EQ(data.model.physicalModels.size(), 5); // Cable, 2 connector, 2 bounds (pec and pml)

    auto& materialCableList = data.model.physicalModels.getOf<PhysicalModel::Wire::Wire>();
    EXPECT_EQ(materialCableList.size(), 1);

    auto& materialPortList = data.model.physicalModels.getOf<PhysicalModel::Multiport::RLC>();
    EXPECT_EQ(materialPortList.size(), 1);

    auto& materialCable = materialCableList.front();
    auto& materialPort = materialPortList.front();

    Geometry::ElemView elementsWithCableMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialCable->getId()) {
            elementsWithCableMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithCableMaterial.size(), 2);

    Geometry::ElemView elementsWithPortMaterial;
    for (auto& elem : data.model.mesh.elems()) {
        if (elem->getMatId() == materialPort->getId()) {
            elementsWithPortMaterial.push_back(elem.get());
        }
    }

    EXPECT_EQ(elementsWithPortMaterial.size(), 2);
}