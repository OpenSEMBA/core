

#pragma once

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class SurfaceBase : public virtual Base {
public:
    SurfaceBase() = default;
    virtual ~SurfaceBase() = default;

    virtual Math::CVecR3 getNormal() const = 0;
};

template<class T>
class Surface : public virtual Element<T>,
                public virtual SurfaceBase {
public:
    Surface() = default;
    virtual ~Surface() = default;

    bool isRectangular() const;
    bool isContainedInPlane() const;
    bool isContainedInPlane(const Math::Constants::CartesianPlane plane) const;

    virtual Math::CVecR3 getNormal() const override; 
};

template<class T>
bool Surface<T>::isRectangular() const {
    if (this->numberOfCoordinates() != 4 || this->numberOfFaces() != 4) {
        return false;
    }
    for (std::size_t f = 0; f < 4; f++) {
        Math::CartesianVector<T, 3> p0 = this->getSideVertex(f % 4, 0)->pos();
        Math::CartesianVector<T, 3> p1 = this->getSideVertex(f % 4, 1)->pos();
        Math::CartesianVector<T, 3> p2 = this->getSideVertex((f + 1) % 4, 1)->pos();
        Math::Real sProd = (Math::Real)(p2 - p1).dot(p1 - p0);
        if (Math::Util::greater(sProd, 0.0, 1.0)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool Surface<T>::isContainedInPlane() const {
    return (isContainedInPlane(Math::Constants::xy) ||
        isContainedInPlane(Math::Constants::yz) ||
        isContainedInPlane(Math::Constants::zx));
}

template<class T>
bool Surface<T>::isContainedInPlane(const Math::Constants::CartesianPlane plane) const {
    for (std::size_t i = 1; i < this->numberOfCoordinates(); i++) {
        if (!(*this->getV(i) - *this->getV(0)).isContainedInPlane(plane)) {
            return false;
        }
    }
    return true;
}

template<class T>
Math::CVecR3 Surface<T>::getNormal() const
{
    Math::CVecR3 v0 = this->getVertex(1)->pos() - this->getVertex(0)->pos();
    Math::CVecR3 v1 = this->getVertex(2)->pos() - this->getVertex(0)->pos();
    return (v0 ^ v1).normalize();
}

} /* namespace Element */

typedef Element::SurfaceBase         Surf;
typedef Element::Surface<Math::Real> SurfR;
typedef Element::Surface<Math::Int > SurfI;

} /* namespace Geometry */
} /* namespace SEMBA */
