

#include <physicalModel/multiport/RLC.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Multiport {

RLC::RLC(const Id id,
                           const std::string name,
                           const Multiport::Type type,
                           const Math::Real resistance,
                           const Math::Real inductance,
                           const Math::Real capacitance)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    type_ = type;
    R_ = resistance;
    L_ = inductance;
    C_ = capacitance;
}

RLC::RLC(const RLC& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    type_ = rhs.type_;
    R_ = rhs.R_;
    L_ = rhs.L_;
    C_ = rhs.C_;
}

RLC::~RLC() {

}

Math::Real RLC::getR() const {
    return R_;
}

Math::Real RLC::getL() const {
    return L_;
}

Math::Real RLC::getC() const {
    return C_;
}

void RLC::printInfo() const {
    std::cout<< " --- Multiport RLC Info --- " << std::endl;
    Multiport::printInfo();
    std::cout<< " Type: " << getTypeStr() << std::endl;
    std::cout<< " Resistance: " << R_ << std::endl;
    std::cout<< " Inductance: " << L_ << std::endl;
    std::cout<< " Capacitance: " << C_ << std::endl;
}

} /* namespace Multiport */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
