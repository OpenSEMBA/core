
#include <gtest/gtest.h>

#include "core/source/port/WaveguideRectangular.h"
#include "core/geometry/element/Quadrilateral4.h"
#include "core/physicalModel/Group.h"
#include "core/physicalModel/Bound.h"
#include "core/math/function/Gaussian.h"

using namespace SEMBA;
using namespace Source;
using namespace PhysicalModel;
using namespace std;

class SourcePortWaveguideRectangularTest : public ::testing::Test {
    void SetUp() {
        Geometry::BoxI3 plane(math::CVecI3(0,0,0), math::CVecI3(30,10,0));
        vector<Geometry::BoxI3> quadBoxes = plane.chop();
        Geometry::ElemId id(0);
        for (size_t i = 0; i < quadBoxes.size(); i++) {
            std::vector<const Geometry::CoordI3*> coords;
            for (size_t j = 0; j < Geometry::Qua4::sizeOfCoordinates; j++) {
                coords.push_back(
                    cG_.addAndAssignId(
                        std::make_unique<Geometry::CoordI3>(
                            Geometry::CoordId(),
                            quadBoxes[i].getPos()[j]
                            )
                    )->get()
                );
            }

            surfs.add(
                std::make_unique<Geometry::QuaI4>(++id, coords.data())
            );
        }

        excMode = Port::Waveguide::ExcitationMode::TE;
        mode = pair<size_t,size_t>(1,0);

        boundType.add( std::make_unique <Bound>(MatId(1), Bound::Type::pml));
        boundType.add( std::make_unique <Bound>(MatId(2), Bound::Type::pmc));
        boundType.add( std::make_unique <Bound>(MatId(3), Bound::Type::pec));

        bounds[0][0] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[1][1] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[2][0] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[0][1] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[1][0] = boundType.getId(MatId(1))->castTo<Bound>();
        bounds[2][1] = boundType.getId(MatId(1))->castTo<Bound>();
    }

protected:
    Geometry::CoordI3Group cG_;
    Geometry::Element::Group<Geometry::Surf> surfs;
    Port::Waveguide::ExcitationMode excMode;
    pair<size_t,size_t> mode;
    Port::Bound3 bounds;
    PMGroup boundType;

    static const std::unique_ptr<Magnitude::Magnitude> buildMagnitude() {
        return std::make_unique<Magnitude::Magnitude>(Magnitude::Magnitude(
            new math::function::Gaussian(
                math::function::Gaussian::buildFromMaximumFrequency(
                    1e9,
                    1.0
                )
            )
        ));
    }
};

//TEST_F(SourcePortWaveguideRectangularTest, withoutSymmetries) 
//{
//    Port::WaveguideRectangular wp{
//        buildMagnitude(), 
//        {surfs.getOf<Geometry::Elem>().front()->getId()},
//        excMode, 
//        mode
//    };
//
//    EXPECT_EQ(math::CVecR3(0.0), wp.getOrigin());
//    EXPECT_EQ(30.0, wp.getWidth());
//    EXPECT_EQ(10.0, wp.getHeight());
//
//    math::CVecR3 midPoint = wp.getOrigin();
//    midPoint(math::Constants::x) = wp.getWidth() / 2.0;
//
//    EXPECT_EQ(math::CVecR3(0.0,1.0,0.0), wp.getWeight(midPoint));
//
//    EXPECT_EQ(math::CVecR3(0.0), wp.getWeight(wp.getOrigin()));
//}