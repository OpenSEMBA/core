#pragma once

#include "geometry/coordinate/Conformal.h"

#include "Line2.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class LineConformal : public virtual Line2<Math::Int> {
public:
    LineConformal(const Id id,
                  std::array<const Coordinate::Coordinate<Math::Int,3>*,2>,
                  const Math::CVecR3& norm,
                  const Layer* lay = nullptr,
                  const Model* mat = nullptr);
    LineConformal(const Id id,
                  const Coordinate::Coordinate<Math::Int, 3>* v[2],
                  const Math::CVecR3& norm,
                  const Layer* lay = nullptr,
                  const Model* mat = nullptr);
    LineConformal(std::array<const Coordinate::Coordinate<Math::Int, 3>*, 2>,
                  const Math::CVecR3& norm,
                  const Layer* lay = nullptr,
                  const Model* mat = nullptr);
    LineConformal(const LineConformal& rhs);
    virtual ~LineConformal() = default;

    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<LineConformal>(*this);
    }

    Math::CVecR3 getNorm () const { return norm_;  }

    const CoordConf* getV(const std::size_t i) const override;

    void setV(const std::size_t i, const CoordI3* coord) override;

    std::unique_ptr<ElemI> toStructured(const CoordI3Group&,
        const Grid3&,
        const Math::Real = Grid3::tolerance) const override;
    std::unique_ptr<ElemR> toUnstructured(const CoordR3Group&,
        const Grid3&) const override;

private:
    void checkCoordinates();
    Math::CVecR3 norm_;
};

} /* namespace Element */

typedef Element::LineConformal LinConf;

} /* namespace Geometry */
} /* namespace SEMBA */

