#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "core/outputRequest/BulkCurrent.h"
#include "core/outputRequest/Domain.h"
#include "core/outputRequest/FarField.h"
#include "core/outputRequest/OnLayer.h"
#include "core/outputRequest/OnLine.h"
#include "core/outputRequest/OnPoint.h"
#include "core/outputRequest/OnSurface.h"
#include "core/outputRequest/OutputRequest.h"

using namespace semba;
using namespace outputRequest;
using namespace geometry;

class OutputRequestTest : public ::testing::Test {

    void SetUp() {}

    void TearDown() {}

};

TEST_F(OutputRequestTest, getOutputRequestTypeStrForName) {
    {
        Domain domain;
        OutputRequest::Target target;
        OnLayer onLayer(OutputRequest::Type::magnetic, domain, "onLayer", target);
        OnLine onLine(OutputRequest::Type::magnetic, domain, "onLayer", target);
        OnSurface onSurface(OutputRequest::Type::magnetic, domain, "onLayer", target);

        FarField farField(domain, "FarField", target, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        OnPoint onPoint(OutputRequest::Type::magnetic, domain, "onPoint", target);
        BulkCurrent bulk(domain, "bulkCurrent", target, math::Constants::CartesianAxis::x, 0);

        EXPECT_THAT(onLayer.getTypeStrForName(), ::testing::StrEq(std::string()));
        EXPECT_THAT(onLine.getTypeStrForName(), ::testing::StrEq(std::string()));
        EXPECT_THAT(onSurface.getTypeStrForName(), ::testing::StrEq(std::string()));

        EXPECT_THAT(farField.getTypeStrForName(), ::testing::StrEq(std::string("Far_Field")));
        EXPECT_THAT(onPoint.getTypeStrForName(), ::testing::StrEq(std::string("On_Point")));
        EXPECT_THAT(bulk.getTypeStrForName(), ::testing::StrEq(std::string("Bulk_Current")));
    }
}
