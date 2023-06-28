#pragma once

#include "core/math/simplex/Line.h"
#include "Element.h"

namespace semba {
namespace geometry {
namespace Element {

class LineBase : public virtual Base {
public:    
    virtual ~LineBase() = default;

    inline std::size_t numberOfFaces   () const { return 2; }
    inline std::size_t numberOfVertices() const { return 2; }

    inline std::size_t numberOfSideVertices   (const std::size_t f = 0) const { 
        return 1; 
    }
    inline std::size_t numberOfSideCoordinates(const std::size_t f = 0) const { 
        return 1; 
    }

};

template<class T>
class Line : public virtual Element<T>,
             public virtual LineBase {
public:
    virtual ~Line() = default;
};

} 

typedef Element::LineBase         Lin;
typedef Element::Line<math::Real> LinR;
typedef Element::Line<math::Int > LinI;

} 
} 


