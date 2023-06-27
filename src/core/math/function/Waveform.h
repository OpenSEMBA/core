#pragma once

#include <complex>
#include <limits>

#include "Function.h"

namespace semba {
    namespace math {
        namespace function {

            class Waveform : public Function<Real, Real> {
            public:
                enum class Type {
                    A,
                    D,
                    H
                };
                Waveform(Type type);
                
                SEMBA_MATH_FUNCTION_DEFINE_CLONE(Waveform);

                Real operator()(const Real& t) const;
                bool operator==(const Base& rhs) const;

            private:
                Real I0_;
                Real alpha_;
                Real beta_;
                Real gamma_;
            };

        } 
    } 
} 

