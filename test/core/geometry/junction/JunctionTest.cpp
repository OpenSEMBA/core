#include "gtest/gtest.h"
#include "gmock/gmock.h"	
#include "gmock/gmock-matchers.h"

#include "core/geometry/junction/Junction.h"

using namespace semba::geometry;

TEST(JunctionTest, CanCreate) {
	std::string junctionName = "test_junction";
	junction::Junction junction(junctionName);

	EXPECT_THAT(junctionName, ::testing::StrEq(junction.getName()));
	EXPECT_TRUE(junction.getUnitedCoordIds().empty());
}
TEST(JunctionTest, CanAddUnitedCoordIds) {
	int coordIdOne = 2;
	int coordIdTwo = 5;
	int coordIdThree = 6;

	std::string junctionName = "test_junction";
	junction::Junction junction(junctionName);
	junction.addCoordinatesToUnite({
				junction::CoordIdForJunctions(coordIdOne),
				junction::CoordIdForJunctions(coordIdTwo),
				junction::CoordIdForJunctions(coordIdThree)
	});

	const auto coordIds = junction.getUnitedCoordIds();

	EXPECT_THAT(junctionName, ::testing::StrEq(junction.getName()));
	EXPECT_EQ(1, coordIds.size());
	EXPECT_THAT(coordIds[0], ::testing::ElementsAre(
		junction::CoordIdForJunctions(coordIdOne),
		junction::CoordIdForJunctions(coordIdTwo),
		junction::CoordIdForJunctions(coordIdThree)
	));
}
TEST(JunctionTest, CanAddIntegersAsUnitedCoordIds) {
	int coordIdOne = 2;
	int coordIdTwo = 5;
	int coordIdThree = 6;

	std::string junctionName = "test_junction";
	junction::Junction junction(junctionName);
	junction.addCoordinatesToUnite({ coordIdOne, coordIdTwo, coordIdThree});

	const auto coordIds = junction.getUnitedCoordIds();

	EXPECT_THAT(junctionName, ::testing::StrEq(junction.getName()));
	EXPECT_EQ(1, coordIds.size());
	EXPECT_THAT(coordIds[0], ::testing::ElementsAre(
		junction::CoordIdForJunctions(coordIdOne),
		junction::CoordIdForJunctions(coordIdTwo),
		junction::CoordIdForJunctions(coordIdThree)
	));
}

TEST(JunctionTest, ThrowsInvalidArgumentException) {
	int coordIdOne = 2;

	junction::Junction junction(std::string("test_junction"));

	EXPECT_THROW({
		try
		{

			junction.addCoordinatesToUnite({ junction::CoordIdForJunctions(coordIdOne) });

		}
		catch (const std::invalid_argument& e)
		{
			EXPECT_STREQ(junction::Junction::INVALID_ATTRIBUTE_MESSAGE.c_str(), e.what());
			throw e;
		}

		}, std::invalid_argument);
}