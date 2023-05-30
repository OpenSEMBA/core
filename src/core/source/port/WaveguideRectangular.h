#pragma once

#include "Waveguide.h"
#include "core/physicalModel/Bound.h"

namespace SEMBA {
namespace Source {
namespace Port {

class WaveguideRectangular: public Waveguide {
public:
    WaveguideRectangular(
            const std::unique_ptr<Magnitude::Magnitude>& magnitude,
            const Target& elem,
            const ExcitationMode excMode,
            const std::pair<size_t,size_t> mode);
    WaveguideRectangular(const WaveguideRectangular&) = default;
    virtual ~WaveguideRectangular() = default;

    virtual std::unique_ptr<Source> clone() const override {
        return std::make_unique<WaveguideRectangular>(*this);
    }

    //void set(const Target&);

    std::string getName() const override;
    //Math::Real getWidth() const;
    //Math::Real getHeight() const;

    //Math::CVecR3 getOrigin() const override;
    //Math::CVecR3 getWeight(const Math::CVecR3& pos) const override;
};

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

