#include "Anisotropic.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

Anisotropic::Anisotropic(const Math::LocalAxis& localAxe) {
    localAxe_ = localAxe;
}

Anisotropic::Anisotropic(const Anisotropic& rhs) {
    localAxe_ = rhs.localAxe_;
}

Anisotropic::~Anisotropic() {

}

Math::LocalAxis Anisotropic::getLocalAxe() const {
    return localAxe_;
}

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
