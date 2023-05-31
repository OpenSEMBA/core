#pragma once

#include <nlohmann/json.hpp>

#include "core/geometry/Grid.h"
#include "core/physicalModel/Bound.h"
#include "core/source/Group.h"
#include "core/outputRequest/Group.h"
#include "core/model/Model.h"
#include "core/util/Project.h"

namespace SEMBA {

template<typename M = UnstructuredModel>
class ProblemDescriptionBase {
public:
	util::Project project;
	Geometry::Grid3 grids;
	SourceGroup sources;
	nlohmann::json analysis;
	M model;
	OutputRequestGroup outputRequests;
};

typedef ProblemDescriptionBase<> UnstructuredProblemDescription;
typedef ProblemDescriptionBase<StructuredModel> StructuredProblemDescription;

}