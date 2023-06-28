#pragma once

#include "Multiport.h"

#include <vector>

namespace semba::physicalModel::multiport {

class MultiWirePort : public virtual Multiport {
public:
    MultiWirePort(
        const Id id,
        const std::string& name,
        const std::vector<math::Real>& resistanceVector);
    MultiWirePort(const MultiWirePort&);
    virtual ~MultiWirePort() = default;

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<MultiWirePort>(*this);
    }

    const std::vector<math::Real>& getResistanceVector() const;
private:
    std::vector<math::Real> resistanceVector;

};

}

