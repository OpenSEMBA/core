#include "MultiWirePort.h"

namespace semba::physicalModel::multiport {
    MultiWirePort::MultiWirePort(
        const Id id,
        const std::string& name,
        const std::vector<math::Real>& resistanceVector) :
        Multiport(id, name, Multiport::Type::multiWireConnector),
        Identifiable<Id>(id),
        PhysicalModel(name),
        resistanceVector(resistanceVector) {}

    MultiWirePort::MultiWirePort(const MultiWirePort& rhs) :
        Multiport(rhs.getId(), rhs.getName(), rhs.getType()),
        Identifiable<Id>(rhs.getId()),
        PhysicalModel(rhs.getName())
    {
        resistanceVector = rhs.resistanceVector;
    }


    const std::vector<math::Real>& MultiWirePort::getResistanceVector() const {
        return resistanceVector;
    }
}