

#pragma once

#include "Line.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Line2Base : public virtual LineBase {
public:
    static const std::size_t sizeOfCoordinates = 2;

    virtual ~Line2Base() = default;

    inline std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }
};

template<class T>
class Line2 : public virtual Line<T>,
              public virtual Line2Base {
public:
    Line2(const Id id,
          const Coordinate::Coordinate<T,3>* v[2],
          const Layer* lay = nullptr,
          const Model* mat = nullptr);
	Line2(const Id id,
		std::array<const Coordinate::Coordinate<T, 3>*,2> v,
		const Layer* lay = nullptr,
		const Model* mat = nullptr);
    // TODO: Remove plain array
    Line2(const Coordinate::Coordinate<T,3>* v[2]);
    Line2(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
          const Box<T,3>& box);
    Line2(const Line2<T>& rhs);
    virtual ~Line2() = default;
    
    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<Line2>(*this);
    }

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>* coord);

    std::unique_ptr<ElemI> toStructured(const CoordI3Group&,
        const Grid3&,
        const Math::Real = Grid3::tolerance) const;
    std::unique_ptr<ElemR> toUnstructured(const CoordR3Group&,
                          const Grid3&) const;

    std::vector<std::unique_ptr<const Line2<T>>> splitByMiddle() const;

private:
    static const Math::Simplex::Line<1> lin;
    // TODO: Remove plain array
    const Coordinate::Coordinate<T,3>* v_[2];

    // TODO: Remove plain array
    void setCoordinates(const Coordinate::Coordinate<T,3>* v[2]);
    void setCoordinates(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
                        const Box<T,3>& box);
};

} /* namespace Element */

typedef Element::Line2Base         Lin2;
typedef Element::Line2<Math::Real> LinR2;
typedef Element::Line2<Math::Int> LinI2;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Line2.hpp"

