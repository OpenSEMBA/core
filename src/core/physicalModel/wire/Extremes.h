

#pragma once

#include "Wire.h"
#include "physicalModel/multiport/Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

class Extremes : public virtual Wire  {
public:
    Extremes(const std::string&,
             const Wire& wire,
             const Multiport::Multiport* extremeL,
             const Multiport::Multiport* extremeR);
    Extremes(const Extremes& rhs);
    virtual ~Extremes();

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<Extremes>(*this);
    }

    const Multiport::Multiport *getExtreme(const std::size_t i) const {
        return extreme_[i];
    }

    void setExtreme(const std::size_t i, const Multiport::Multiport* extreme);
    void swapExtremes();

private:
    const Multiport::Multiport* extreme_[2];
};

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

