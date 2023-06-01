#include "Port.h"

namespace semba {
namespace Source {
namespace Port {

Port::Port(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
           const Target& elem) :   
    Source(magnitude, elem) 
{}

}
}
} 

