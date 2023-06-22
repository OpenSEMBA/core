#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "core/geometry/Layer.h"
#include "core/geometry/element/Polyline.h"

using namespace semba;

TEST(PolylineTest, CanCreate) {
	geometry::CoordR3 vertexOne{ geometry::CoordId(), math::CVecR3({0.0, 0.0, 0.0}) };
	geometry::CoordR3 vertexTwo{ geometry::CoordId(), math::CVecR3({1.0, 0.0, 0.0}) };
	geometry::CoordR3 vertexThree{ geometry::CoordId(), math::CVecR3({2.0, 0.0, 0.0}) };

	geometry::Layer lay{geometry::LayerId(), "My layer"};
	geometry::element::Model model{MatId()};

	geometry::PolylinR poly{ geometry::ElemId(), {&vertexOne, &vertexTwo, &vertexThree}, &lay, &model };
	
	EXPECT_THAT(poly.getCoordinates(), ::testing::ElementsAre(&vertexOne, &vertexTwo, &vertexThree));

}
