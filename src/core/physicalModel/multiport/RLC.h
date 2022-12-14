

#pragma once

#include "Multiport.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

class RLC : public virtual Multiport {
public:
    RLC(const Id idIn,
                 const std::string nameIn,
                 const Multiport::Type typeIn,
                 const Math::Real resistance,
                 const Math::Real inductance,
                 const Math::Real capacitance);
    RLC(const RLC&);
    virtual ~RLC();

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<RLC>(*this);
    }

    virtual Math::Real getR() const;
    virtual Math::Real getL() const;
    virtual Math::Real getC() const;
private:
    Math::Real R_, L_, C_;
};

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

