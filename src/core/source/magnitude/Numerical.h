#pragma once

#include "Magnitude.h"
#include "core/filesystem/Project.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

class Numerical : public virtual Magnitude {
public:
    Numerical() = default;
    Numerical(const Numerical&) = default;
    Numerical(const FileSystem::Project& filename);
    Numerical(const FileSystem::Project& filename,
              const Magnitude& mag,
              const math::Real timeStep,
              const math::Real finalTime);
    
    std::unique_ptr<Magnitude> clone() const override {
        return std::make_unique<Numerical>(*this);
    }

    bool operator==(const Numerical&) const;
    math::Real evaluate(const math::Real time) const;

    FileSystem::Project getFile() const { return file; };

private:
    static const std::size_t defaultNumberOfSteps = 1000;
    FileSystem::Project file;

};

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */

