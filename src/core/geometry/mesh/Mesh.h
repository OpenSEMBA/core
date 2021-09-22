

#pragma once

#include "math/Types.h"
#include "geometry/Box.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"

#include "geometry/element/Element.h"
#include "geometry/element/Group.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Mesh : public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Shareable,
             public virtual Class::Printable {
public:
    Mesh();
    virtual ~Mesh();

    virtual void applyScalingFactor(const Math::Real factor) = 0;
    virtual BoxR3 getBoundingBox() const = 0;
    virtual void reassignPointers(
            const SEMBA::Group::Identifiable<Element::Model, MatId>& =
                SEMBA::Group::Identifiable<Element::Model, MatId>()) = 0;

    virtual void reassign( Element::Group<const Elem>& ) = 0;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

