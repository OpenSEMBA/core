

#pragma once

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class NodeBase : public virtual Base {
public:
	static const std::size_t sizeOfCoordinates = 1;

    NodeBase() = default;
    virtual ~NodeBase() = default;

    inline std::size_t numberOfCoordinates() const { return 1; }
    inline std::size_t numberOfFaces   () const { return 1; }
    inline std::size_t numberOfVertices() const { return 1; }
    inline std::size_t numberOfSideVertices   (const std::size_t f = 0) const {
        return 1;
    }
    inline std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 1;
    }
};

template<class T>
class Node : public virtual Element<T>,
             public virtual NodeBase {
public:
    Node() = default;
    Node(const Id id,
         const Coordinate::Coordinate<T,3>* v[1],
         const Layer* lay = nullptr,
         const Model* mat = nullptr);
    Node(const Node<T>& rhs);
    Node& operator=(const Node<T>& rhs);

    Node(Node<T>&&) = default;
    Node& operator=(Node&&) = default;

    virtual ~Node() = default;
    
    virtual std::unique_ptr<Base> clone() const override {
        return std::make_unique<Node>(*this);
    }

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const override;

    const Coordinate::Coordinate<T,3>* getV(const std::size_t i) const override;
    const Coordinate::Coordinate<T,3>* getSideV(
        const std::size_t f, const std::size_t i) const override;

    const Coordinate::Coordinate<T,3>* getVertex(const std::size_t i) const override;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f, const std::size_t i) const override;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>* coord) override;

    std::unique_ptr<ElemI> toStructured(const CoordI3Group&, const Grid3&,
                        const Math::Real = Grid3::tolerance) const override;
    std::unique_ptr<ElemR> toUnstructured(const CoordR3Group&, const Grid3&) const override;

private:
    std::array<const Coordinate::Coordinate<T,3>*, 1> v_;
};

template<class T>
Node<T>::Node(const Id id,
    const Coordinate::Coordinate<T, 3>* v[1],
    const Layer* lay,
    const Model* mat)
    : Identifiable<Id>(id),
    Elem(lay, mat) {

    v_[0] = v[0];
}

template<class T>
Node<T>::Node(const Node<T>& rhs)
    : Identifiable<Id>(rhs),
      Elem(rhs) 
{
    v_[0] = rhs.v_[0];
}

template<class T>
Node<T>& Node<T>::operator=(const Node<T>& rhs) :  
    Identifiable<Id>(rhs),
    Elem(rhs)
{
    v_[0] = rhs.v_[0];
}

template<class T>
bool Node<T>::isStructured(const Grid3& grid, const Math::Real tol) const 
{
    if (!this->vertexInCell(grid, tol)) {
        return false;
    }
    if (!this->getBound().isPoint()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
}

template<class T>
const Coordinate::Coordinate<T, 3>* Node<T>::getV(const std::size_t i) const 
{
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T, 3>* Node<T>::getSideV(
    const std::size_t f,
    const std::size_t i) const {
    assert(f == 0 && i == 0);
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T, 3>* Node<T>::getVertex(
    const std::size_t i) const {
    assert(i == 0);
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T, 3>* Node<T>::getSideVertex(
    const std::size_t f,
    const std::size_t i) const {
    assert(f == 0 && i == 0);
    return v_[i];
}

template<class T>
void Node<T>::setV(const std::size_t i,
    const Coordinate::Coordinate<T, 3>* coord) {
    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
std::unique_ptr<ElemI> Node<T>::toStructured(
    const CoordI3Group& cG,
    const Grid3& grid,
    const Math::Real tol) const {
    return std::make_unique<NodI>(this->getId(),
        this->vertexToStructured(cG, grid, tol).data(),
        this->getLayer(),
        this->getModel());
}

template<class T>
std::unique_ptr<ElemR> Node<T>::toUnstructured(
    const CoordR3Group& cG,
    const Grid3& grid) const {
    return std::make_unique<NodR>(this->getId(),
        this->vertexToUnstructured(cG, grid).data(),
        this->getLayer(),
        this->getModel());
}


} /* namespace Element */

typedef Element::NodeBase         Nod;
typedef Element::Node<Math::Real> NodR;
typedef Element::Node<Math::Int > NodI;

} /* namespace Geometry */
} /* namespace SEMBA */