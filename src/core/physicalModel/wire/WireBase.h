#pragma once

#include "core/physicalModel/PhysicalModel.h"

namespace semba {
namespace physicalModel {
namespace wire {

class WireBase : public virtual PhysicalModel {
protected:
    WireBase() = default;
    WireBase(const Id id,
         const std::string name);
    
    WireBase(const WireBase&);
public:
    virtual ~WireBase() = default;

};

}
} 
} 

