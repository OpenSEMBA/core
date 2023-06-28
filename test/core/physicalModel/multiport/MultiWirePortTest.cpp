#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "core/physicalModel/multiport/MultiWirePort.h"
#include "core/physicalModel/multiport/Multiport.h"

namespace semba::physicalModel::multiport {

    class MultiWirePortTest : public ::testing::Test {
    public:
        const static std::string exampleName;
        const static semba::physicalModel::Id exampleId;
        const static std::vector<math::Real> exampleResistanceVector;
    };

    const semba::physicalModel::Id MultiWirePortTest::exampleId = semba::physicalModel::Id(2);
    const std::string MultiWirePortTest::exampleName = "BundleConnectorName";
    const std::vector<math::Real> MultiWirePortTest::exampleResistanceVector = { 0.0, 1.0 };

    TEST_F(MultiWirePortTest, can_create)
    {
        MultiWirePort multiWireConnector(
            MultiWirePortTest::exampleId,
            MultiWirePortTest::exampleName,
            MultiWirePortTest::exampleResistanceVector
        );

        EXPECT_EQ(MultiWirePortTest::exampleId, multiWireConnector.getId());
        EXPECT_EQ(MultiWirePortTest::exampleName, multiWireConnector.getName());
        EXPECT_EQ(Multiport::Type::multiWireConnector, multiWireConnector.getType());
        EXPECT_THAT(multiWireConnector.getResistanceVector(), testing::ContainerEq(MultiWirePortTest::exampleResistanceVector));
    }

    TEST_F(MultiWirePortTest, copy_assignment)
    {
        MultiWirePort orig(
            MultiWirePortTest::exampleId,
            MultiWirePortTest::exampleName,
            MultiWirePortTest::exampleResistanceVector
        );

        MultiWirePort copied = orig;

        EXPECT_EQ(MultiWirePortTest::exampleId, copied.getId());
        EXPECT_EQ(MultiWirePortTest::exampleName, copied.getName());
        EXPECT_EQ(Multiport::Type::multiWireConnector, copied.getType());
        EXPECT_THAT(orig.getResistanceVector(), testing::ContainerEq(copied.getResistanceVector()));
    }

    TEST_F(MultiWirePortTest, copy_ctor)
    {
        MultiWirePort orig(
            MultiWirePortTest::exampleId,
            MultiWirePortTest::exampleName,
            MultiWirePortTest::exampleResistanceVector
        );
        MultiWirePort copied(orig);

        EXPECT_EQ(MultiWirePortTest::exampleId, copied.getId());
        EXPECT_EQ(MultiWirePortTest::exampleName, copied.getName());
        EXPECT_EQ(Multiport::Type::multiWireConnector, copied.getType());
        EXPECT_THAT(orig.getResistanceVector(), testing::ContainerEq(copied.getResistanceVector()));
    }

}