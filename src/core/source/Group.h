#pragma once

#include "core/source/Source.h"
#include "core/util/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace Source {

template<typename S = Source>
class Group : public util::GroupIdentifiableUnique<S> {
};

} /* namespace Source */

using SourceGroup = Source::Group<>;

} /* namespace SEMBA */
