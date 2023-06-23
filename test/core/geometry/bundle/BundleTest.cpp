#include "gtest/gtest.h"
#include "gmock/gmock.h"	
#include "gmock/gmock-matchers.h"

#include "core/geometry/bundle/Bundle.h"

using namespace semba::geometry;

TEST(BundleTest, CanCreate) {
	std::string bundleName = "test_bundle";
	int wireMaterialIdNumber = 1;
	int initialConnectorMaterialIdNumber = 3;
	int endConnectorMaterialIdNumber = 7;
	int polylineElementOneIdNumber = 4;
	int polylineElementTwoIdNumber = 5;

	bundle::Bundle bundle{
		bundleName,
		bundle::MatIdForBundle(wireMaterialIdNumber),
		bundle::MatIdForBundle(initialConnectorMaterialIdNumber),
		bundle::MatIdForBundle(endConnectorMaterialIdNumber),
		{bundle::ElemIdForBundle(polylineElementOneIdNumber), bundle::ElemIdForBundle(polylineElementTwoIdNumber)}
	};

	EXPECT_THAT(bundleName, ::testing::StrEq(bundle.getName()));
	EXPECT_EQ(bundle::MatIdForBundle(wireMaterialIdNumber), bundle.getMaterialId());
	EXPECT_EQ(bundle::MatIdForBundle(initialConnectorMaterialIdNumber), bundle.getInitialConnectorId());
	EXPECT_EQ(bundle::MatIdForBundle(endConnectorMaterialIdNumber), bundle.getEndConnectorId());
	EXPECT_THAT(bundle.getElemIds(), ::testing::ElementsAre(
		bundle::ElemIdForBundle(polylineElementOneIdNumber),
		bundle::ElemIdForBundle(polylineElementTwoIdNumber)
	));
}