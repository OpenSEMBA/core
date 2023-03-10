#include "Port.h"

namespace SEMBA {
namespace Source {
namespace Port {

Port::Port(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
           const Target& elem) :   
    Source(magnitude, elem) 
{}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */

