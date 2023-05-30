#pragma once

#include "Volume.h"
#include "core/math/LocalAxis.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

class Anisotropic : public virtual Volume {
public:
    enum class Model {
        crystal,
        ferrite
    };

    Anisotropic(const Math::LocalAxis& local);
    Anisotropic(const Anisotropic& rhs);
    virtual ~Anisotropic();

    Math::LocalAxis getLocalAxe() const;
    virtual Math::MatR33 getRelPermittivityMatR() const = 0;
    virtual Math::MatR33 getRelPermeabilityMatR() const = 0;
    virtual Math::MatR33 getElectricConductivityMat() const = 0;
    virtual Math::MatR33 getMagneticConductivityMat() const = 0;

private:
    Math::LocalAxis localAxe_;
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

