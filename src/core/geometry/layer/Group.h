#pragma once

#include "Layer.h"
#include "core/util/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace Geometry {
namespace Layer {

template<typename L = Layer>
class Group final : public util::GroupIdentifiableUnique<L>  {
public:
    const L* getName(const std::string name) const;

};

template<typename L>
const L* Group<L>::getName(const std::string name) const {
    for (auto const& layer : this->items_) {
        if (layer->get()->getName() == name) {
            return layer->get();
        }
    }
    return nullptr;
}

} /* namespace Layer */

typedef Layer::Group<> LayerGroup;

} /* namespace Geometry */
} /* namespace SEMBA */

