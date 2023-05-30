#pragma once

#include <memory>

#include "core/geometry/Grid.h"
#include "core/math/vector/Cartesian.h"
#include "core/class/Class.h"
#include "core/class/Identifiable.h"
#include "core/class/Identification.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

class Base;
typedef Class::Identification<Base> Id;

class Base : public virtual Class::Class,
             public virtual Class::Identifiable<Id> {
public:
    Base() = default;
    virtual ~Base() = default;
    
    virtual std::unique_ptr<Base> clone() const = 0;

    virtual bool operator==(const Base& rhs) const;
    virtual bool operator!=(const Base& rhs) const;
};

template <class T, std::size_t D>
class Coordinate : public virtual Base,
                   public virtual Math::Vector::Cartesian<T,D> {
public:
    Coordinate() = default;
    Coordinate(const Id id_, const Math::Vector::Cartesian<T,D>& pos);
    explicit Coordinate(const Math::Vector::Cartesian<T,D>& pos);
    Coordinate(const Coordinate& rhs);
    virtual ~Coordinate() = default;

    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<Coordinate<T, D>>(*this);
    }

    Coordinate& operator=(const Coordinate& rhs);

    bool operator==(const Base& rhs) const override;
    bool operator!=(const Base& rhs) const override;

    virtual bool isStructured(const Grid<D>&,
                              const Math::Real = Grid<D>::tolerance) const;

    Math::Vector::Cartesian<T,D>&       pos()       { return *this; }
    const Math::Vector::Cartesian<T,D>& pos() const { return *this; }

    virtual Coordinate<Math::Int ,D>* toStructured  (const Grid<D>&) const;
    virtual Coordinate<Math::Real,D>* toUnstructured(const Grid<D>&) const;

};


template<class T, std::size_t D>
Coordinate<T, D>::Coordinate(const Id id,
    const Math::Vector::Cartesian<T, D>& pos)
    : Identifiable<Id>(id),
    Math::Vector::Cartesian<T, D>(pos) {

}

template<class T, std::size_t D>
Coordinate<T, D>::Coordinate(const Math::Vector::Cartesian<T, D>& pos)
    : Math::Vector::Cartesian<T, D>(pos) {

}

template<class T, std::size_t D>
Coordinate<T, D>::Coordinate(const Coordinate& rhs)
    : Identifiable<Id>(rhs),
    Math::Vector::Cartesian<T, D>(rhs) {

}

template<class T, std::size_t D>
Coordinate<T, D>& Coordinate<T, D>::operator=(const Coordinate& rhs)
{
    setId(rhs.getId());
    Math::Vector::Cartesian<T, D>::operator=(rhs);
    return *this;
}

template<class T, std::size_t D>
bool Coordinate<T, D>::operator==(const Base& rhs) const {
    if (!Base::operator==(rhs)) {
        return false;
    }
    const Coordinate<T, D>* rhsPtr = rhs.castTo<Coordinate<T, D> >();
    bool res = true;
    res &= (this->getId() == rhsPtr->getId());
    res &= (this->pos() == rhsPtr->pos());
    return res;
}

template<class T, std::size_t D>
bool Coordinate<T, D>::operator!=(const Base& rhs) const {
    return Base::operator!=(rhs);
}

template<class T, std::size_t D>
bool Coordinate<T, D>::isStructured(const Grid<D>& grid,
    const Math::Real tol) const {
    if (!grid.isCell(*this, tol)) {
        return false;
    }
    return true;
}

template<class T, std::size_t D>
Coordinate<Math::Int, D>* Coordinate<T, D>::toStructured(
    const Grid<D>& grid) const {
    return new Coordinate<Math::Int, D>(this->getId(), grid.getCell(*this));
}

template<class T, std::size_t D>
Coordinate<Math::Real, D>* Coordinate<T, D>::toUnstructured(
    const Grid<D>& grid) const {
    return new Coordinate<Math::Real, D>(this->getId(), grid.getPos(*this));
}


} /* namespace Coordinate */

typedef Coordinate::Id                       CoordId;
typedef Coordinate::Base                     Coord;
typedef Coordinate::Coordinate<Math::Real,3> CoordR3;
typedef Coordinate::Coordinate<Math::Int ,3> CoordI3;

} /* namespace Geometry */
} /* namespace SEMBA */


