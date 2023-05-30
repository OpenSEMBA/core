#pragma once

#include <string>
#include <memory>

#include "core/util/Identifiable.h"
#include "core/util/Identification.h"

namespace SEMBA {
namespace Geometry {
namespace Layer {

class Layer;
typedef util::Identification<Layer> Id;

class Layer final : public virtual util::Class,
              public virtual util::Identifiable<Id> {
public:
    Layer() = default;
    Layer(const Id id, const std::string& name);
    Layer(const std::string& name);
    Layer(const Layer& rhs);
    
    std::unique_ptr<Layer> clone() const {
        return std::make_unique<Layer>(*this);
    }
    
    virtual bool operator==(const Layer& rhs) const;
    virtual bool operator!=(const Layer& rhs) const;

    std::string getName() const;

    virtual std::string getParentName() const;
    virtual std::string getChildName() const;
    std::string toStr() const;

    friend std::ostream& operator<<(std::ostream& os, const Layer& lay) {
       return os << lay.toStr();
    }

private:
    std::string name_;

    static std::string spaceToUnderscore(std::string rhs);
};



} /* namespace Layer */

typedef Layer::Id LayerId;

} /* namespace Geometry */
} /* namespace SEMBA */

