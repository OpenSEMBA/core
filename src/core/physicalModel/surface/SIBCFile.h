#pragma once

#include "core/math/matrix/Static.h"
#include "core/util/Project.h"

#include "Surface.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

class SIBCFile : public virtual Surface {
public:
    SIBCFile();
    SIBCFile(const Id id,
             const std::string& name,
             const util::Project& file);
    
    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<SIBCFile>(*this);
    }

    const util::Project getFile() const;

protected:
    util::Project file_;
};

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

