#pragma once

#include "OutputRequest.h"
#include "core/util/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace OutputRequest {

template<typename O = OutputRequest>
class Group : public util::GroupIdentifiableUnique<O> {
};

} /* namespace OutputRequest */

typedef OutputRequest::Group<> OutputRequestGroup;

} /* namespace SEMBA */

