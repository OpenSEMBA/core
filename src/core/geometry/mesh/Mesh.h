#pragma once

#include "core/math/Types.h"
#include "core/geometry/Box.h"
#include "core/class/Class.h"
#include "core/class/Identifiable.h"
#include "core/class/Identification.h"
#include "core/geometry/element/Element.h"
#include "core/geometry/element/Group.h"
#include "core/physicalModel/Group.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Mesh;
    typedef Class::Identification<Mesh> Id;

class Mesh : public virtual Class::Identifiable<Id>,
             public virtual Class::Class {
public:
    Mesh() = default;
    virtual ~Mesh() = default;

    virtual void applyScalingFactor(const math::Real factor) = 0;
    virtual BoxR3 getBoundingBox() const = 0;
    virtual void reassignPointers(const PMGroup& matGr = PMGroup()) = 0;

    virtual std::unique_ptr<Mesh> clone() const = 0;

    virtual ElemView reassign(const ElemView&) = 0;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

