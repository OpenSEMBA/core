#include "gtest/gtest.h"

#include "junction/Junction.h"

using namespace SEMBA;

TEST(JunctionTest, CanCreate) {
	junction::Junction<Math::Real, 3> junction("test_junction");
	junction.addPairOfCoordinatesToUnite(
		junction::CoordIdForJunctions(0), 
		junction::CoordIdForJunctions(1)
	);
	EXPECT_EQ(
		1,
		1
	);
}