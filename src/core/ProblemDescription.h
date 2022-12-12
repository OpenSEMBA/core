#pragma once

#include "geometry/Grid.h"
#include "physicalModel/Bound.h"
#include "source/Group.h"
#include "outputRequest/Group.h"
#include "model/Model.h"

#include "nlohmann/json.hpp"
#include "filesystem/Project.h"

namespace SEMBA {

template<typename M = Model::UnstructuredModel>
class ProblemDescriptionBase {
public:
	FileSystem::Project project;
	Geometry::Grid3 grids;
	SourceGroup sources;
	nlohmann::json analysis;

	M model;

	OutputRequestGroup outputRequests;

	ProblemDescriptionBase() = default;
	ProblemDescriptionBase(const ProblemDescriptionBase& rhs);

	ProblemDescriptionBase& operator=(const ProblemDescriptionBase& rhs);
};

typedef ProblemDescriptionBase<> UnstructuredProblemDescription;
typedef ProblemDescriptionBase<Model::StructuredModel> StructuredProblemDescription;

template<typename M>
ProblemDescriptionBase<M>::ProblemDescriptionBase(const ProblemDescriptionBase& rhs) {
    grids = rhs.grids;
    analysis = rhs.analysis;
    project = rhs.project;

    model = rhs.model;

    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        outputRequest->setTarget(
            model.mesh.reassign(outputRequest->getTarget())
        );
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        source->setTarget(
            model.mesh.reassign(source->getTarget())
        );
    }
}

template<typename M>
ProblemDescriptionBase<M>& ProblemDescriptionBase<M>::operator=(const ProblemDescriptionBase& rhs) {
    grids = rhs.grids;
    analysis = rhs.analysis;
    project = rhs.project;

    model = rhs.model;

    outputRequests = rhs.outputRequests;
    for (auto& outputRequest : outputRequests) {
        outputRequest->setTarget(
            model.mesh.reassign(outputRequest->getTarget())
        );
    }

    sources = rhs.sources;
    for (auto& source : sources) {
        source->setTarget(
            model.mesh.reassign(source->getTarget())
        );
    }

    return *this;
}

}