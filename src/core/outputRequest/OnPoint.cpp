#include "OnPoint.h"

namespace semba {
namespace OutputRequest {

    OnPoint::OnPoint(
        const Type& outputType,
        const Domain& domain,
        const std::string& name,
        const Target& elem
    )
        : OutputRequest(outputType, name, domain, elem)
    {}

} 
} 
