#pragma once

#include "core/source/Source.h"
#include "core/class/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace Source {

template<typename S = Source>
class Group : public Class::Group::GroupIdentifiableUnique<S> {
};

} /* namespace Source */

using SourceGroup = Source::Group<>;

} /* namespace SEMBA */
