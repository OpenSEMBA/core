#include "Waveform.h"

#include <cmath>
#include <iostream>

namespace semba {
    namespace math {
        namespace function {
            Waveform::Waveform(Type type) {
                if (type == Type::A) {
                    I0_    =   218810.0;
                    alpha_ =    11354.0;
                    beta_  =   647265.0;
                    gamma_ =  5423540.0;
                } else if (type == Type::D) {
                    I0_    =   109405.0;
                    alpha_ =    22708.0;
                    beta_  =  1294530.0;
                    gamma_ = 10847100.0;
                } else if (type == Type::H) {
                    I0_    =    10572.0;
                    alpha_ =   187191.0;
                    beta_  = 19105100.0;
                    gamma_ = 15330600.0;
                } else {
                    throw std::logic_error("Invalid Waveform type");
                }
            }

            Waveform::Type Waveform::getEnumValue(const std::string& str) {
                {
                    if (str == "A") return Waveform::Type::A;
                    else if (str == "D") return Waveform::Type::D;
                    else if (str == "H") return Waveform::Type::H;
                }
            }

            Real Waveform::operator ()(const Real& t) const {
                return I0_ * 
                    (std::exp(-alpha_*t) - std::exp(-beta_*t)) *
                    std::pow((1 - std::exp(-gamma_*t)), 2);
            }

            bool Waveform::operator==(const Base& rhs) const {
                if (typeid(*this) != typeid(rhs)) {
                    return false;
                }
                const Waveform* rhsPtr = dynamic_cast<const Waveform*>(&rhs);
                return this->I0_ == rhsPtr->I0_ &&
                    this->alpha_ == rhsPtr->alpha_ &&
                    this->beta_ == rhsPtr->beta_ &&
                    this->gamma_ == rhsPtr->gamma_;
            }

        } 
    } 
} 
