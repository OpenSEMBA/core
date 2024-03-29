

#include <physicalModel/volume/Dispersive.h>

namespace semba {
namespace physicalModel {
namespace volume {

Dispersive::Dispersive(const Id id,
                                   const std::string& name,
                                   const math::Real rEps,
                                   const math::Real rMu,
                                   const math::Real elecCond,
                                   const math::Real magnCond,
                                   const std::vector<PoleResidue>& poleResidue)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    rEpsInfty_ = rEps;
    rMuInfty_ = rMu;
    // Adds conductivity as a permittivity pole.
    if (elecCond != 0.0) {
        std::complex<math::Real> pole(0.0);
        std::complex<math::Real> residue(elecCond / math::Real(2.0) /
                                         math::Constants::eps0, 0);
        poleResidue_.push_back(PoleResidue(pole, residue));
    }
    //
    if (magnCond != 0.0) {
        throw std::logic_error("Dispersive magnetic materials not implemented");
    }
    poleResidue_ = poleResidue;
}

Dispersive::Dispersive(const Id id,
                       const std::string& name,
                       const util::ProjectFile& file)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
	rEpsInfty_ = 1.0;
    rMuInfty_ = 1.0;
	if (!file.canOpen()) {
		throw std::ios_base::failure(std::string("File: ") + file +
			std::string(" does not exist"));
	}
    file_ = file;
}

Dispersive::Dispersive(const Dispersive& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    rEpsInfty_ = rhs.rEpsInfty_;
    rMuInfty_ = rhs.rMuInfty_;
    poleResidue_ = rhs.poleResidue_;
    file_ = rhs.file_;
}

Dispersive::~Dispersive() {

}

std::size_t Dispersive::getPoleNumber() const {
    return poleResidue_.size();
}

std::complex<math::Real> Dispersive::getPole(std::size_t p) const {
    return poleResidue_[p].first;
}

std::complex<math::Real> Dispersive::getResidue(std::size_t p) const {
    return poleResidue_[p].second;
}

bool Dispersive::isDispersive() const {
    if (poleResidue_.size() > 0) {
        return true;
    }
    return false;
}

bool Dispersive::isClassic() const {
    return isSimplyConductive();
}

bool Dispersive::isSimplyConductive() const {
    if (!file_.empty()) {
        return false;
    }
    return (poleResidue_.size() <= 1 && std::abs(getPole(0)) == 0);
}

math::Real Dispersive::getElectricConductivity() const {
    if (getPoleNumber() > 1) {
        std::cout << std::endl << "WARNING @ getElectricConductivity: "
                  << "This material is dispersive and its effective "
                  << "permittivity depends on several parameters. "
                  << "Returning static limit conductivity." << std::endl;
    }
    for (std::size_t i = 0; i < getPoleNumber(); i++) {
        if (std::abs(getPole(i)) == 0) {
            return getResidue(i).real() * 2.0 * math::Constants::eps0;
        }
    }
    return 0.0;
}

void Dispersive::addPole(
        const std::complex<math::Real>& pole,
        const std::complex<math::Real>& res) {
    poleResidue_.push_back(PoleResidue(pole,res));
    return;
}

const util::ProjectFile Dispersive::getFile() const {
    return file_;
}


}
} 
} 
