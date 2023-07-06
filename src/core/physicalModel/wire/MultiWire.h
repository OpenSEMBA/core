#pragma once

#include "WireBase.h"
#include <vector>

namespace semba::physicalModel::wire {
class MultiWire : public WireBase {
public:
    MultiWire(
        const Id id,
        const std::string & name,
        const std::vector<math::Real> & resistanceVector,    
        const std::vector< std::vector<math::Real> >& inductanceMatrix,
        const std::vector< std::vector<math::Real> >& capacitanceMatrix);

    MultiWire(const MultiWire&);
    virtual ~MultiWire() = default;
    
    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<MultiWire>(*this);
    }

    inline size_t getNumberOfWires() const { return resistanceVector.size(); }
    const std::vector<math::Real>& getResistanceVector() const;
    const std::vector< std::vector<math::Real> >& getInductanceMatrix() const;
    const std::vector< std::vector<math::Real> >& getCapacitanceMatrix() const;

private:
    std::vector<math::Real> resistanceVector;
    std::vector< std::vector<math::Real> > inductanceMatrix;
    std::vector< std::vector<math::Real> > capacitanceMatrix;

    static void assertDimensions(
        const std::vector<math::Real>& resistanceVector,
        const std::vector< std::vector<math::Real> >& inductanceMatrix,
        const std::vector< std::vector<math::Real> >& capacitanceMatrix);
};


}
