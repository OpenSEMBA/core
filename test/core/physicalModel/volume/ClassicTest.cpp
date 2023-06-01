#include <gtest/gtest.h>
#include "core/physicalModel/volume/Classic.h"

using namespace semba;
using namespace PhysicalModel;

class PhysicalModelVolumeClassicTest : public ::testing::Test {
public:

protected:

};

TEST_F(PhysicalModelVolumeClassicTest, ctor) {
    EXPECT_NO_THROW(Volume::Classic(Id(1), "Classic", 1.0, 1.0, 0.0, 0.0));
}

TEST_F(PhysicalModelVolumeClassicTest, isVacuum) {
    Volume::Classic vacuum(Id(1), "Vacuum", 1.0, 1.0, 0.0, 0.0);
    EXPECT_TRUE(vacuum.isVacuum());
}
