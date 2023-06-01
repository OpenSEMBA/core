#include "OnLayer.h"

namespace semba {
namespace OutputRequest {

    OnLayer::OnLayer(
        const Type& outputType,
        const Domain& domain,
        const std::string& name,
        const Target& elem
    )
        : OutputRequest(outputType, name, domain, elem)
    {}

} 
} 
