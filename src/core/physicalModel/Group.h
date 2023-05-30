#pragma once

#include "PhysicalModel.h"
#include "core/util/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace PhysicalModel {

template<typename P = PhysicalModel>
class Group : public util::GroupIdentifiableUnique<P> {
};

} /* namespace PhysicalModel */

typedef PhysicalModel::Group<> PMGroup;

} /* namespace SEMBA */

