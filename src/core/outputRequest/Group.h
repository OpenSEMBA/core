#pragma once

#include "OutputRequest.h"
#include "core/util/GroupIdentifiableUnique.h"

namespace semba {
namespace OutputRequest {

template<typename O = OutputRequest>
class Group : public util::GroupIdentifiableUnique<O> {
};

} 

typedef OutputRequest::Group<> OutputRequestGroup;

} 

