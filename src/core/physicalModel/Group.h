#pragma once

#include "PhysicalModel.h"
#include "core/util/GroupIdentifiableUnique.h"

namespace semba {
namespace PhysicalModel {

template<typename P = PhysicalModel>
class Group : public util::GroupIdentifiableUnique<P> {
};

} 

typedef PhysicalModel::Group<> PMGroup;

} 

