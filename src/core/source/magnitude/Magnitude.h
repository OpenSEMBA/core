#pragma once

#include "core/math/function/Function.h"
#include "core/class/Class.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

class Magnitude : public virtual Class::Class {
public:
    Magnitude() = default;
    Magnitude(math::FunctionRR* mathFunction);
    Magnitude(const Magnitude& rhs);
    virtual ~Magnitude() = default;

    Magnitude& operator=(const Magnitude& rhs);

    virtual std::unique_ptr<Magnitude> clone() const {
        return std::make_unique<Magnitude>(*this);
    }

    virtual bool operator==(const Magnitude&) const;

    math::Real evaluate(const math::Real time) const;

private:
    math::FunctionRR* mathFunction_;
};

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */

