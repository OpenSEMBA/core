#include "Dipole.h"

namespace SEMBA {
namespace Source {

Dipole::Dipole(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
               const Target& elem,
               math::Real length,
               math::CVecR3 orientation,
               math::CVecR3 position)
:   Source(magnitude, elem)
{
    length_ = length;
    orientation_ = orientation;
    position_ = position;
}


} /* namespace Source */
} /* namespace SEMBA */
