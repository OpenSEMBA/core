#include "OnSurface.h"

namespace semba {
namespace OutputRequest {

OnSurface::OnSurface(
    const Type& outputType,
    const Domain& domain,
    const std::string& name,
    const Target& elem
) : OutputRequest(outputType, name, domain, elem)
{}

} 
} 
