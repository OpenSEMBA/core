

#pragma once

#include "Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class Tetrahedron4 : public Tetrahedron {
public:
    static const std::size_t sizeOfCoordinates = 4;

    Tetrahedron4(const Id id,
                 const CoordR3* v[4],
                 const Layer* lay = nullptr,
                 const Model* mat = nullptr);
    Tetrahedron4(const Tetrahedron4& rhs);
    virtual ~Tetrahedron4() = default;

    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<Tetrahedron4>(*this);
    }

    bool isInnerPoint(const Math::CVecR3& pos) const;
    bool isCurvedFace(const std::size_t face) const;
    bool isFaceContainedInPlane(
            const std::size_t face,
            const Math::Constants::CartesianPlane plane) const;

    std::size_t numberOfCoordinates() const { return sizeOfCoordinates; }

    std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 3;
    }

    const CoordR3* getV(const std::size_t i) const { return v_[i]; }
    const CoordR3* getSideV(const std::size_t f, const std::size_t i) const;

    const CoordR3* getVertex(const std::size_t i) const {
        return v_[tet.vertex(i)];
    }
    const CoordR3* getSideVertex(const std::size_t f,
                                 const std::size_t i) const;

    Math::Real getVolume() const;
    const Math::Simplex::Simplex& getTet() const { return tet; }
    Math::Real getAreaOfFace(const std::size_t face) const;

    void setV(const std::size_t i, const CoordR3*);
    void check() const;

    virtual std::unique_ptr<Element<Math::Int >> toStructured(
        const CoordI3Group&,
        const Grid3&,
        const Math::Real = Grid3::tolerance) const;

    virtual std::unique_ptr<Element<Math::Real>> toUnstructured(
        const CoordR3Group&,
        const Grid3&) const;

private:
    static const Math::Simplex::Tetrahedron<1> tet;

    // TODO: Remove plain array
    const CoordR3* v_[4];

    bool hasZeroVolume() const;
};

} /* namespace Element */

typedef Element::Tetrahedron4 Tet4;

} /* namespace Geometry */
} /* namespace SEMBA */

