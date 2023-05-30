#pragma once

#include <complex>
#include <limits>

#include "Function.h"

namespace SEMBA {
namespace math {
namespace function {

    class BandLimited : public Function<Real, Real> {
    public:
        BandLimited(Real minimumFrequency, Real maximumFrequency);
                
        SEMBA_MATH_FUNCTION_DEFINE_CLONE(BandLimited);

        Real operator()(const Real& t) const;
        bool operator==(const Base& rhs) const;
    private:
        Real minimumFrequency_;
        Real maximumFrequency_;
    };

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */

