#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"

#include "core/physicalModel/wire/MultiWire.h"

using namespace semba;
using namespace physicalModel;

class PhysicalModelMultiWireTest : public ::testing::Test {
public:
    const static std::string exampleName;
    const static semba::physicalModel::Id exampleId;
    const static std::vector<math::Real> exampleResistanceVector;
    const static std::vector< std::vector<math::Real> > exampleInductanceMatrix;
    const static std::vector< std::vector<math::Real> > exampleCapacitanceMatrix;
};

const semba::physicalModel::Id PhysicalModelMultiWireTest::exampleId = semba::physicalModel::Id(2);
const std::string PhysicalModelMultiWireTest::exampleName = "Multicable";
const std::vector<math::Real> PhysicalModelMultiWireTest::exampleResistanceVector = { 0.0, 1.0 };
const std::vector< std::vector<math::Real> > PhysicalModelMultiWireTest::exampleInductanceMatrix =
{   { 1.0, 2.0 },
    { 3.0, 4.0 } 
};
const std::vector< std::vector<math::Real> > PhysicalModelMultiWireTest::exampleCapacitanceMatrix =
{   { 1.5, 2.5 },
    { 3.5, 4.5 }
};


TEST_F(PhysicalModelMultiWireTest, can_create) 
{
        wire::MultiWire multiWire(
            PhysicalModelMultiWireTest::exampleId,
            PhysicalModelMultiWireTest::exampleName,
            PhysicalModelMultiWireTest::exampleResistanceVector,
            PhysicalModelMultiWireTest::exampleInductanceMatrix,
            PhysicalModelMultiWireTest::exampleCapacitanceMatrix
        );

        EXPECT_EQ(PhysicalModelMultiWireTest::exampleId, multiWire.getId());
        EXPECT_EQ(PhysicalModelMultiWireTest::exampleName, multiWire.getName());
        EXPECT_THAT(multiWire.getResistanceVector(), testing::ContainerEq(PhysicalModelMultiWireTest::exampleResistanceVector));
        EXPECT_THAT(multiWire.getInductanceMatrix(), testing::ContainerEq(PhysicalModelMultiWireTest::exampleInductanceMatrix));
        EXPECT_THAT(multiWire.getCapacitanceMatrix(), testing::ContainerEq(PhysicalModelMultiWireTest::exampleCapacitanceMatrix));
}

TEST_F(PhysicalModelMultiWireTest, throws_length_error_when_creating_with_dispar_dimensions) {
    std::vector< std::vector<math::Real> > wrongInductanceMatrix =
    {
        {1.0},
        {2.5, 3.7},
        {4.4, 5.5, 6.6}
    };
    std::vector< std::vector<math::Real> > wrongCapacitanceMatrix =
    {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.4, 8.5, 9.6}
    };

    EXPECT_THROW({
        try
        {
            wire::MultiWire(
               PhysicalModelMultiWireTest::exampleId,
                PhysicalModelMultiWireTest::exampleName,
                PhysicalModelMultiWireTest::exampleResistanceVector,
                wrongInductanceMatrix,
                wrongCapacitanceMatrix);

        }
        catch (const std::length_error& e)
        {
            std::string expectedError = "All vectors and matrixes must share the same size!\n"
                "Resistance Vector size: 2\n"
                "Inductance Matrix sizes:\n"
                "- rows: 3\n"
                "- size of each row : [1, 2, 3]\n"
                "Capacitance Matrix sizes:\n"
                "- rows: 3\n"
                "- size of each row : 3";

            EXPECT_STREQ(expectedError.c_str(), e.what());
            throw e;
        }

        }, std::length_error);
}

TEST_F(PhysicalModelMultiWireTest, copy_assignment) 
{
    wire::MultiWire orig(
        PhysicalModelMultiWireTest::exampleId,
        PhysicalModelMultiWireTest::exampleName,
        PhysicalModelMultiWireTest::exampleResistanceVector,
        PhysicalModelMultiWireTest::exampleInductanceMatrix,
        PhysicalModelMultiWireTest::exampleCapacitanceMatrix
    );

    wire::MultiWire copied = orig;

    EXPECT_EQ(PhysicalModelMultiWireTest::exampleId, copied.getId());
    EXPECT_EQ(PhysicalModelMultiWireTest::exampleName, copied.getName());
    EXPECT_THAT(orig.getResistanceVector(), testing::ContainerEq(copied.getResistanceVector()));
    EXPECT_THAT(orig.getInductanceMatrix(), testing::ContainerEq(copied.getInductanceMatrix()));
    EXPECT_THAT(orig.getCapacitanceMatrix(), testing::ContainerEq(copied.getCapacitanceMatrix()));
}

TEST_F(PhysicalModelMultiWireTest, copy_ctor)
{
    wire::MultiWire orig(
        PhysicalModelMultiWireTest::exampleId,
        PhysicalModelMultiWireTest::exampleName,
        PhysicalModelMultiWireTest::exampleResistanceVector,
        PhysicalModelMultiWireTest::exampleInductanceMatrix,
        PhysicalModelMultiWireTest::exampleCapacitanceMatrix
    );

    wire::MultiWire copied(orig);

    EXPECT_EQ(PhysicalModelMultiWireTest::exampleId, copied.getId());
    EXPECT_EQ(PhysicalModelMultiWireTest::exampleName, copied.getName());
    EXPECT_THAT(orig.getResistanceVector(), testing::ContainerEq(copied.getResistanceVector()));
    EXPECT_THAT(orig.getInductanceMatrix(), testing::ContainerEq(copied.getInductanceMatrix()));
    EXPECT_THAT(orig.getCapacitanceMatrix(), testing::ContainerEq(copied.getCapacitanceMatrix()));
}