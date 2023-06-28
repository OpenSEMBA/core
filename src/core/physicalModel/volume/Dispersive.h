#pragma once

#include <complex>
#include <exception>
#include <utility>

#include "core/filesystem/Project.h"
#include "Volume.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

typedef std::pair<std::complex<math::Real>,
                  std::complex<math::Real>> PoleResidue;

class Dispersive : public virtual Volume {
public:
    Dispersive(const Id id,
                     const std::string& name,
                     const math::Real rEps,
                     const math::Real rMu,
                     const math::Real elecCond,
                     const math::Real magnCond,
                     const std::vector<PoleResidue>& poleResidue =
                        std::vector<PoleResidue>());
    Dispersive(const Id id,
                     const std::string& name,
                     const FileSystem::Project& file);
    Dispersive(const Dispersive& rhs);
    virtual ~Dispersive();

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<Dispersive>(*this);
    }

    std::size_t getPoleNumber() const;
    std::complex<math::Real> getPole(std::size_t p) const;
    std::complex<math::Real> getResidue(std::size_t p) const;
    virtual math::Real getElectricConductivity() const;

    bool isClassic() const;
    bool isSimplyConductive() const;
    bool isDispersive() const;

    const FileSystem::Project getFile() const;

protected:
    math::Real rEpsInfty_, rMuInfty_; // @ InftyFreq.
    std::vector<PoleResidue> poleResidue_; // Residues for dispers model. c_p.
    FileSystem::Project file_;
    void addPole(const std::complex<math::Real>& pole_,
                 const std::complex<math::Real>& res_);
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

