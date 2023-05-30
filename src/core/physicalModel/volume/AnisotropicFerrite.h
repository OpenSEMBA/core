#pragma once

#include "Anisotropic.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

// Described using Polder tensor model:
// https://en.wikipedia.org/wiki/Polder_tensor
class AnisotropicFerrite: public Anisotropic {
public:
    AnisotropicFerrite(const Id matId,
                             const std::string& name,
                             const Math::LocalAxis& local,
                             const Math::Real kappa,
                             const Math::Real relativePermeability,
                             const Math::Real relativePermittivity);
    AnisotropicFerrite(const AnisotropicFerrite&);
    virtual ~AnisotropicFerrite();

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<AnisotropicFerrite>(*this);
    }

    Math::MatR33 getRelPermittivityMatR() const override;

    Math::MatR33 getRelPermeabilityMatR() const override; // Math::Real part.
    Math::MatR33 getRelPermeabilityMatI() const; // Imaginary part.

    Math::MatR33 getElectricConductivityMat() const override;
    Math::MatR33 getMagneticConductivityMat() const override;
private:
    Math::Real kappa_;
    Math::Real relativePermeability_;
    Math::Real relativePermittivity_;
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

