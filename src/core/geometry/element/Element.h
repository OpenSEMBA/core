#pragma once

#include "core/math/CartesianVector.h"
#include "core/physicalModel/Group.h"
#include "core/geometry/Box.h"
#include "core/geometry/Grid.h"
#include "core/geometry/coordinate/Group.h"
#include "core/geometry/layer/Group.h"
#include "core/class/Class.h"
#include "core/class/Identifiable.h"
#include "core/class/Identification.h"

#include <algorithm>

namespace SEMBA {

typedef PhysicalModel::Id MatId;

namespace Geometry {
namespace Element {

typedef Geometry::Layer::Layer     Layer;
typedef Class::Identifiable<MatId> Model;

class Base;
typedef Class::Identification<Base> Id;

class Base : public virtual Class::Identifiable<Id>,
             public virtual Class::Class {
public:
    Base(const Layer* lay = nullptr,
         const Model* mat = nullptr);
    Base(const Base& rhs);
    virtual ~Base() = default;

    virtual bool operator==(const Base& rhs) const;
    virtual bool operator!=(const Base& rhs) const;

    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }

    virtual std::size_t numberOfFaces      () const = 0;
    virtual std::size_t numberOfVertices   () const = 0;
    virtual std::size_t numberOfCoordinates() const = 0;

    virtual std::size_t numberOfSideVertices(const std::size_t f = 0) const = 0;
    virtual std::size_t numberOfSideCoordinates(const std::size_t f = 0) const = 0;

    LayerId getLayerId() const;
    MatId   getMatId  () const;

    const Layer* getLayer() const { return lay_; }
    const Model* getModel() const { return mat_; }

    static std::vector<CoordId> ascendingIdOrder(const std::vector<CoordId>& rhs);

    template<class T1, class T2>
    static bool areSameCoords(
            const std::vector<const T1*>& lhs,
            const std::vector<const T2*>& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        std::vector<CoordId> lhsId, rhsId;
        for (std::size_t i = 0; i < lhs.size(); i++) {
            lhsId.push_back(lhs[i]->getId());
            rhsId.push_back(rhs[i]->getId());
        }
        return (ascendingIdOrder(lhsId) == ascendingIdOrder(rhsId));
    }

    template<class T>
    static std::vector<CoordId> getIds(
            std::vector<const Coordinate::Coordinate<T,3>*> in) {
        std::vector<CoordId> res(in.size());
        for  (std::size_t i = 0; i < in.size(); i++) {
            res[i] = in[i]->getId();
        }
        return res;
    }

    virtual void setLayer(const Layer* lay) { lay_ = lay; }
    virtual void setModel(const Model* mat) { mat_ = mat; }

    virtual std::unique_ptr<Base> clone() const = 0;

private:
    const Layer* lay_;
    const Model* mat_;
};

template<class T>
class Element : public virtual Base {
public:
    Element() = default;
    virtual ~Element() = default;

    bool operator== (const Base& rhs) const;

    bool isCoordinate(const Coordinate::Coordinate<T,3>* coord) const;

    virtual bool isStructured(const Grid3&,
                              const math::Real = Grid3::tolerance) const;
    virtual bool isInnerPoint(const math::CartesianVector<T,3>& pos) const;

    virtual const Coordinate::Coordinate<T,3>* getV(const std::size_t i) const = 0;
    virtual const Coordinate::Coordinate<T,3>* getSideV(
            const std::size_t f, const std::size_t i) const = 0;

    virtual const Coordinate::Coordinate<T,3>* getVertex(
            const std::size_t i) const = 0;
    virtual const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f, const std::size_t i) const = 0;

    Box<T,3> getBound() const;
    // Returns ptr to coord with min(max) lexicographical position.
    virtual const Coordinate::Coordinate<T,3>* getMinV() const;
    virtual const Coordinate::Coordinate<T,3>* getMaxV() const;

    std::vector<const Coordinate::Coordinate<T,3>*>getVertices() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getCoordinates() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideCoordinates(
            const std::size_t face) const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideVertices(
            const std::size_t face) const;

    virtual void setV(const std::size_t i, const Coordinate::Coordinate<T,3>*);

    virtual std::unique_ptr<Element<math::Int >> toStructured(
            const CoordI3Group&,
            const Grid3&,
            const math::Real = Grid3::tolerance) const = 0;
    virtual std::unique_ptr<Element<math::Real>> toUnstructured(
            const CoordR3Group&,
            const Grid3&) const = 0;

protected:
    bool vertexInCell (const Grid3& grid, const math::Real tol) const;
    bool vertexInBound() const;
    std::vector<const CoordI3*> vertexToStructured(const CoordI3Group& cG,
                                       const Grid3& grid,
                                       const math::Real tol) const;
    std::vector<const CoordR3*> vertexToUnstructured(const CoordR3Group& cG,
                                         const Grid3& grid) const;

};

template<class T>
bool Element<T>::operator==(const Base& rhs) const {
    if (!Base::operator==(rhs)) {
        return false;
    }
    const Element<T>* rhsPtr = rhs.castTo<Element<T>>();
    bool res = true;
    res &= (this->getId() == rhsPtr->getId());
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        res &= (*this->getV(i) == *rhsPtr->getV(i));
    }
    res &= (this->getLayerId() == rhsPtr->getLayerId());
    res &= (this->getMatId() == rhsPtr->getMatId());

    return res;
}

template<class T>
bool Element<T>::isCoordinate(const Coordinate::Coordinate<T, 3>* coord) const {
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        if (*getV(i) == *coord) {
            return true;
        }
    }
    return false;
}

template<class T>
bool Element<T>::isStructured(const Grid3& grid, const math::Real tol) const {
    return false;
}

template<class T>
bool Element<T>::isInnerPoint(const math::CartesianVector<T, 3>& pos) const {
    throw std::logic_error("Element::isInnerPoint not implemented");
    return false;
}

template<class T>
Box<T, 3> Element<T>::getBound() const {
    Box<T, 3> res;
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        res << getV(i)->pos();
    }
    return res;
}

template<class T>
const Coordinate::Coordinate<T, 3>* Element<T>::getMinV() const {
    assert(getV(0) != nullptr);
    const Coordinate::Coordinate<T, 3>* res = getVertex(0);
    for (std::size_t i = 1; i < numberOfVertices(); i++) {
        if (res->pos() == getVertex(i)->pos()) {
            continue;
        }
        for (std::size_t j = 0; j < 3; j++) {
            math::Real val1 = getVertex(i)->pos()(j);
            math::Real val2 = res->pos()(j);
            if (math::Util::lower(val1, val2, res->pos().norm())) {
                res = getVertex(i);
                break;
            }
        }
    }
    return res;
}

template<class T>
const Coordinate::Coordinate<T, 3>* Element<T>::getMaxV() const {
    assert(getV(0) != nullptr);
    const Coordinate::Coordinate<T, 3>* res = getVertex(0);
    for (std::size_t i = 1; i < numberOfVertices(); i++) {
        if (res->pos() == getVertex(i)->pos()) {
            continue;
        }
        for (std::size_t j = 0; j < 3; j++) {
            math::Real val1 = getVertex(i)->pos()(j);
            math::Real val2 = res->pos()(j);
            if (math::Util::greater(val1, val2, res->pos().norm())) {
                res = getVertex(i);
                break;
            }
        }
    }
    return res;
}

template<class T>
std::vector<const Coordinate::Coordinate<T, 3>*>
Element<T>::getVertices() const {
    std::vector<const Coordinate::Coordinate<T, 3>*> res(numberOfVertices());
    for (std::size_t i = 0; i < numberOfVertices(); i++) {
        res[i] = getVertex(i);
    }
    return res;
}

template<class T>
std::vector<const Coordinate::Coordinate<T, 3>*>
Element<T>::getCoordinates() const {
    std::vector<const Coordinate::Coordinate<T, 3>*> res(numberOfCoordinates());
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        res[i] = getV(i);
    }
    return res;
}

template<class T>
std::vector<const Coordinate::Coordinate<T, 3>*> Element<T>::getSideCoordinates(
    const std::size_t face) const {
    std::vector<const Coordinate::Coordinate<T, 3>*>
        res(numberOfSideCoordinates());
    for (std::size_t i = 0; i < numberOfSideCoordinates(); i++) {
        res[i] = getSideV(face, i);
    }
    return res;
}

template<class T>
std::vector<const Coordinate::Coordinate<T, 3>*> Element<T>::getSideVertices(
    const std::size_t face) const {
    std::vector<const Coordinate::Coordinate<T, 3>*> res;
    res.resize(numberOfSideVertices());
    for (std::size_t i = 0; i < numberOfSideVertices(); i++) {
        res[i] = getSideVertex(face, i);
    }
    return res;
}

template<class T>
void Element<T>::setV(const std::size_t i,
    const Coordinate::Coordinate<T, 3>* coord) {
    throw std::logic_error(
        "Setting coordinates is not allowed for this element");
}

template<class T>
bool Element<T>::vertexInCell(const Grid3& grid, const math::Real tol) const {
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        if (!grid.isCell(*this->getV(i), tol)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool Element<T>::vertexInBound() const {
    Box<T, 3> bound = this->getBound();
    std::vector< math::CartesianVector<T, 3> > pos = bound.getPos();
    if (pos.size() != this->numberOfCoordinates()) {
        return false;
    }
    std::vector<bool> found(pos.size(), false);
    bool foundCoord;
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        foundCoord = false;
        for (std::size_t j = 0; j < pos.size(); j++) {
            if (found[j]) {
                continue;
            }
            if (this->getV(i)->pos() == pos[j]) {
                foundCoord = true;
                found[j] = true;
                break;
            }
        }
        if (!foundCoord) {
            return false;
        }
    }
    return true;
}

template<class T>
std::vector<const CoordI3*> Element<T>::vertexToStructured(
    const CoordI3Group& cG,
    const Grid3& grid,
    const math::Real tol) const {
    if (!this->is<Element<math::Real>>() || !this->isStructured(grid, tol)) {
        throw std::logic_error("Element::vertexToStructured unexpected empty element");
    }

    std::vector<const CoordI3*> res(this->numberOfCoordinates());
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        math::CVecI3 cell = grid.getCell(*this->getV(i), true, tol);
        CoordId coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            throw std::logic_error("Coord not found.");
        }
        res[i] = cG.getId(coordId);
        if (res[i]->pos() != cell) {
            throw std::logic_error("Coord not coincident.");
        }
    }

    return res;
}

template<class T>
std::vector<const CoordR3*> Element<T>::vertexToUnstructured(
    const CoordR3Group& cG,
    const Grid3& grid) const {
    if (!this->is<Element<math::Int>>()) {
        throw std::logic_error("Element::vertexToStructured unexpected empty element");
    }

    std::vector<const CoordR3*> res(this->numberOfCoordinates());
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        CoordId coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            throw std::logic_error("Coord not found");
        }
        res[i] = cG.getId(coordId);
        const CoordR3* unsCoord = this->getV(i)->toUnstructured(grid);
        if (res[i]->pos() != unsCoord->pos()) {
            throw std::logic_error("Coord not coincident");
        }
    }
    return res;
}

typedef Element<math::Real> ElemR;
typedef Element<math::Int>  ElemI;

} /* namespace Element */

typedef Element::Element<math::Real> ElemR;
typedef Element::Element<math::Int>  ElemI;

typedef Element::Id                  ElemId;
typedef Element::Base                Elem;

typedef std::vector<const Geometry::Elem*>  ElemView;

} /* namespace Geometry */
} /* namespace SEMBA */