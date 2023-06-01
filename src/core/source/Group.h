#pragma once

#include "core/source/Source.h"
#include "core/util/GroupIdentifiableUnique.h"

namespace semba {
namespace Source {

template<typename S = Source>
class Group : public util::GroupIdentifiableUnique<S> {
};

}

using SourceGroup = Source::Group<>;

} 
