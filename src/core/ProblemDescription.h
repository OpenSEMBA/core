#pragma once

#include "geometry/Grid.h"
#include "physicalModel/Bound.h"
#include "source/Group.h"
#include "outputRequest/Group.h"
#include "model/Model.h"

#include "nlohmann/json.hpp"
#include "filesystem/Project.h"

namespace SEMBA {

template<typename M = UnstructuredModel>
class ProblemDescriptionBase {
public:
	FileSystem::Project project;
	Geometry::Grid3 grids;
	SourceGroup sources;
	nlohmann::json analysis;
	M model;
	OutputRequestGroup outputRequests;
};

typedef ProblemDescriptionBase<> UnstructuredProblemDescription;
typedef ProblemDescriptionBase<StructuredModel> StructuredProblemDescription;

}