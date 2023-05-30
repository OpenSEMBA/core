#pragma once

#include "core/geometry/element/Group.h"
#include "core/filesystem/Project.h"
#include "core/class/Class.h"
#include "core/class/Identifiable.h"
#include "core/class/Identification.h"
#include "magnitude/Magnitude.h"
#include "magnitude/Numerical.h"

namespace SEMBA {
namespace Source {

class Source;
    typedef Class::Identification<Source> Id;

class Source : public virtual Class::Identifiable<Id>,
               public virtual Class::Class {
public:
    using Target = std::vector<Geometry::ElemId>;

    Source(const std::unique_ptr<Magnitude::Magnitude>&, const Target&);
    Source(const Source&);
    virtual ~Source() = default;

    Source& operator=(const Source&);
    
    virtual std::unique_ptr<Source> clone() const = 0;

    virtual std::string getName() const = 0;

    void convertToNumerical(const FileSystem::Project& file,
                            const Math::Real step,
                            const Math::Real finalTime);
    
    Magnitude::Numerical exportToFile(const FileSystem::Project& file,
                                       const Math::Real step,
                                       const Math::Real finalTime) const;
    const Magnitude::Magnitude* getMagnitude() const { return magnitude_.get(); }

    Target getTarget() const { return target_; }
    void setTarget(const Target& target) { target_ = target; }

protected:
    std::unique_ptr<Magnitude::Magnitude> magnitude_;
    Target target_;
};

} /* namespace Source */
} /* namespace SEMBA */


