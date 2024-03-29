#include "Tetrahedron4.h"

namespace semba {
namespace geometry {
namespace element {

const math::simplex::Tetrahedron<1> Tetrahedron4::tet;

Tetrahedron4::Tetrahedron4(const Id id,
           const CoordR3* v[4],
           const Layer* lay,
           const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    for (std::size_t i = 0; i < tet.np; i++) {
        v_[i] = v[i];
    }
    check();
}

Tetrahedron4::Tetrahedron4(const Id id,
            const std::vector<const CoordR3*>& v,
            const Layer* lay,
            const Model* mat)
:    Identifiable<Id>(id),
     Elem(lay, mat) {

     for (std::size_t i = 0; i < tet.np; i++) {
        v_[i] = v[i];
     }
    check();
}


Tetrahedron4::Tetrahedron4(const Tetrahedron4& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

bool Tetrahedron4::isInnerPoint(const math::CVecR3& pos) const {
    if (!getBound().isInnerPoint(pos)) {
        return false;
    }
    // Checks if point is inside a tetrahedron using the following algorithm:
    // http://steve.hollasch.net/cgindex/geometry/ptintet.html
    math::matrix::Static<math::Real,4,4> mat;
    // Builds matrix D0.
    for (std::size_t i = 0; i < 4; i++) {
        for (std::size_t j = 0; j < 3; j++) {
            mat(i,j) = getVertex(i)->pos()(j);
        }
        mat(i,3) = (math::Real) 1.0;
    }
    math::Real det = mat.getDeterminant4x4();
    assert(det != 0);
    bool isPositive = (det > (math::Real) 0.0);
    // Checks rest of matrices. Last column is always 1.0.
    for (std::size_t k = 0; k < 4; k++) {
        // Copies pos in row k.
        for (std::size_t j = 0; j < 3; j++) {
            mat(k,j) = pos(j);
        }
        // Copies rest of vertices.
        for (std::size_t i = 0; i < 4; i++) {
            if (i != k) {
                for (std::size_t j = 0; j < 3; j++) {
                    mat(i,j) = getVertex(i)->pos()(j);
                }
            }
        }
        math::Real det = mat.getDeterminant4x4();
        if ((det > (math::Real) 0.0) != isPositive) {
            return false;
        }
    }
    return true;
}

bool Tetrahedron4::isCurvedFace(const std::size_t face) const {
    return false;
}

bool Tetrahedron4::isFaceContainedInPlane(
        const std::size_t face,
        const math::Constants::CartesianPlane plane) const {
    return getTri3Face(face)->isContainedInPlane(plane);
}

const CoordR3* Tetrahedron4::getSideV(const std::size_t f,
                                      const std::size_t i) const {
    return v_[tet.sideNode(f,i)];
}

const CoordR3* Tetrahedron4::getSideVertex(const std::size_t f,
                                           const std::size_t i) const {
    return v_[tet.sideVertex(f,i)];
}

math::Real Tetrahedron4::getVolume() const {
    math::matrix::Static<math::Real,3,3> mat;
    math::CVecR3 aux;
    for (std::size_t i = 1; i < 4; i++) {
        aux = getV(0)->pos() - getV(i)->pos();
        for (std::size_t j = 0; j < 3; j++) {
            mat(i-1,j) = aux(j);
        }
    }
    math::Real det = mat.getDeterminant3x3();
    return (det / ((math::Real) 6.0));
}

math::Real Tetrahedron4::getAreaOfFace(const std::size_t f) const {
    math::CVecR3 v1, v2;
    v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
    v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
    return ((math::Real) 0.5 * (v1 ^ v2).norm());
}

void Tetrahedron4::setV(const std::size_t i, const CoordR3* v) {
    v_[i] = v;
}

void Tetrahedron4::check() const {
    if(hasZeroVolume()) {
        throw std::logic_error("Tetrahedron has zero volume");
    }
}

bool Tetrahedron4::hasZeroVolume() const {
    bool zeroVolume;
    math::CVecR3 initialVCoord, otherVCoord;

    // TODO: Remove plain array
    initialVCoord = *v_[0];
    for (std::size_t d = 0; d < 3; d++) {
        zeroVolume = true;
        for (std::size_t i = 1; i < tet.np; i++) {
            otherVCoord = *v_[i];
            zeroVolume &= (initialVCoord(d) == otherVCoord(d));
        }
        if (zeroVolume) {
            return true;
        }
    }
    return false;
}

std::unique_ptr<ElemI> Tetrahedron4::toStructured(
    const CoordI3Group& cG,
    const Grid3& grid, const math::Real tol) const {

    throw std::logic_error("Tetrahedron4::toStructured operation not permitted");
}

std::unique_ptr<ElemR> Tetrahedron4::toUnstructured(
    const CoordR3Group& cG,
    const Grid3& grid) const {
    return std::make_unique<Tet4>(this->getId(),
        this->vertexToUnstructured(cG, grid).data(),
        this->getLayer(),
        this->getModel());
}

} 
} 
} 
