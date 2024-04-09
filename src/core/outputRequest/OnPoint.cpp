#include "OnPoint.h"

namespace semba {
namespace outputRequest {

    OnPoint::OnPoint(
        const Type& outputType,
        const Domain& domain,
        const std::string& name,
        const Target& elem
    )
        : OutputRequest(outputType, name, domain, elem)
    {}

    std::string OnPoint::getTypeStrForName() const
    {
        return std::string("On_Point");
    }
} 
} 
