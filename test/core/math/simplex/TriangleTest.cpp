#include <gtest/gtest.h>
#include "core/math/simplex/Triangle.h"

using namespace SEMBA;
using namespace Math;

template <typename T>
class MathSimplexTriangleTest : public ::testing::Test {

};

TEST(MathSimplexTriangleTest, BasicOperations) {
    static constexpr std::size_t n = 3;
    Simplex::Triangle<n> tri;

    Real sum = 0.0;
    std::vector<Real> weights = tri.getWeights();
    for (size_t i = 0; i < weights.size(); ++i) {
        sum += weights[i];
    }
    EXPECT_NEAR(1.0, sum, 1e-8);
}
