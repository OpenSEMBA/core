#include "gtest/gtest.h"

#include "geometry/Grid.h"
#include "geometry/mesh/Unstructured.h"

#include "source/PlaneWave.h"
#include "math/function/Gaussian.h"
#include "physicalModel/Predefined.h"
#include "outputRequest/OnPoint.h"

#include "ProblemDescription.h"
using namespace SEMBA;
using namespace Geometry;

void fillProblemDescription(UnstructuredProblemDescription& pD)
{

	pD.project = FileSystem::Project("MyPath");
	
	{
		// Create mesh
		CoordR3Group coordinatesGroup;
		coordinatesGroup.copyAndAssignId(CoordR3{ CoordId{1}, { 0.0, 0.0, 0.0 } });
		coordinatesGroup.copyAndAssignId(CoordR3{ CoordId{2}, { 1.0, 2.0, 3.0 } });
	
		ElemRGroup elementsGroup;
		{
			const CoordR3* c[1] = { coordinatesGroup.getId(CoordId(1)) };
			elementsGroup.copyAndAssignId(NodR{ ElemId(1), c });
		}
		{
			const CoordR3* c[1] = { coordinatesGroup.getId(CoordId(2)) };
			elementsGroup.copyAndAssignId(NodR{ ElemId(2), c });
		}
		UnstructuredMesh m{coordinatesGroup, elementsGroup};

		// Create PMGroup.
		PMGroup physicalModelsGroup;
		physicalModelsGroup.copyAndAssignId(PhysicalModel::PEC{PhysicalModel::Id(), "Material PEC"});
		
		pD.model = UnstructuredModel{m, physicalModelsGroup};
	}
	
	// Create source
	pD.sources.copyAndAssignId(
		Source::PlaneWave{
			std::make_unique<Source::Magnitude::Magnitude>(
				new SEMBA::Math::Function::Gaussian(0.5, 0.0, 1.0)
			),
			{ElemId{1}},
			Math::CVecR3(1.0, 0.0, 0.0),
			Math::CVecR3(0.0, 0.0, 1.0)
		}
	);

	// Create OutputRequest
	pD.outputRequests.copyAndAssignId(
		OutputRequest::OnPoint{
			OutputRequest::OutputRequest::Type::electric,
			OutputRequest::Domain(),
			"My electric field point probe",
			{ElemId(2)}
		}
	);

}

auto returnProblemDescription()
{
	UnstructuredProblemDescription pD;
	fillProblemDescription(pD);
	return pD;
}

TEST(ProblemDescriptionTest, CanCreate) 
{
	UnstructuredProblemDescription problemDescription;

	EXPECT_NE(&problemDescription, nullptr);
}

TEST(ProblemDescriptionTest, CanInitializeProject) 
{
	const std::string path {"My/Project/Path/File.dat"};

	UnstructuredProblemDescription problemDescription;
	problemDescription.project = FileSystem::Project(path);

	EXPECT_EQ(problemDescription.project, path);
}

TEST(ProblemDescriptionTest, CanInitializeSources) {
	UnstructuredProblemDescription problemDescription;

	SourceGroup sources;

	Source::PlaneWave planewave{
		std::make_unique<Source::Magnitude::Magnitude>(
			new SEMBA::Math::Function::Gaussian(0.5, 0.0, 1.0)
		),
		{},
		Math::CVecR3(1.0, 0.0, 0.0),
		Math::CVecR3(0.0, 0.0, 1.0)
	};

	sources.addAndAssignId(std::make_unique<Source::PlaneWave>(planewave));

	problemDescription.sources = sources;

	EXPECT_EQ(problemDescription.grids, Grid3());

	EXPECT_EQ(problemDescription.sources.size(), 1);

	auto sourceInGroup = problemDescription.sources.getId(Source::Id(1))->castTo<Source::PlaneWave>();
	EXPECT_EQ(sourceInGroup->getDirection(), planewave.getDirection());
	EXPECT_EQ(sourceInGroup->getPolarization(), planewave.getPolarization());
	
	EXPECT_EQ(*sourceInGroup->getMagnitude(), *planewave.getMagnitude());
}

TEST(ProblemDescriptionTest, CanInitializeModel) {
	UnstructuredProblemDescription problemDescription = UnstructuredProblemDescription();

	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.addAndAssignId(
		std::make_unique<PhysicalModel::PEC>(
			PhysicalModel::Id(),
			"Material PEC"
		)
	);

	const auto& physicalModelIt = physicalModelsGroup.addAndAssignId(
		std::make_unique<PhysicalModel::Bound>(PhysicalModel::Id(), PhysicalModel::Bound::Type::pml)
	);

	CoordR3Group coordinatesGroup = CoordR3Group();
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			CoordId(),
			Math::CVecR3(0.0, 0.0, 0.0)
		)
	);
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			CoordId(),
			Math::CVecR3(1.0, 2.0, 3.0)
		)
	);

	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			CoordId(),
			Math::CVecR3(5.0, 5.0, 5.0)
			)
	);

	const CoordR3* coordinatesArgumentList[1] = { coordinatesGroup.getId(CoordId(1)) };
	ElemRGroup elementsGroup;
	elementsGroup.addAndAssignId(
		std::make_unique<NodR>(
			ElemId(), coordinatesArgumentList
		)
	);

	const CoordR3* coordinatesArgumentList2[1] = { coordinatesGroup.getId(CoordId(2)) };
	elementsGroup.addAndAssignId(
		std::make_unique<NodR>(
			ElemId(),
			coordinatesArgumentList2
		)
	);

	// Boundaries
	const CoordR3* coordinatesArgumentBoundaryList[1] = { coordinatesGroup.getId(CoordId(3)) };
	elementsGroup.addAndAssignId(
		std::make_unique<NodR>(
			ElemId(),
			coordinatesArgumentBoundaryList,
			nullptr,
			physicalModelIt->get()
		)
	);

	const UnstructuredModel model{
		Mesh::Unstructured(coordinatesGroup, elementsGroup),
		physicalModelsGroup
	};

	problemDescription.model = model;

	ASSERT_FALSE(problemDescription.model.physicalModels.empty());
	EXPECT_EQ("Material PEC", problemDescription.model.physicalModels.get()[0]->getName());
	EXPECT_EQ("PML_Bound", problemDescription.model.physicalModels.get()[1]->getName());

	EXPECT_FALSE(problemDescription.model.mesh.coords().empty());
	EXPECT_EQ(
		Math::CVecR3(1.0, 2.0, 3.0),
		(problemDescription.model.mesh.coords().get()[1])->pos()
	);

	const auto& element = problemDescription.model.mesh.elems().getId(ElemId(3));

	EXPECT_EQ(
		element->getMatId(),
		problemDescription.model.physicalModels.get()[1]->getId()
	);

	EXPECT_EQ(
		element->getCoordinates()[0]->pos(),
		Math::CVecR3(5.0, 5.0, 5.0)
	);
}

TEST(ProblemDescriptionTest, CanCopyConstructor) 
{
	UnstructuredProblemDescription pD;
	fillProblemDescription(pD);
	
	UnstructuredProblemDescription copy{ pD };

	EXPECT_EQ(copy.project, pD.project);
}

TEST(ProblemDescriptionTest, CanAssign)
{
	UnstructuredProblemDescription pD;
	fillProblemDescription(pD);

	UnstructuredProblemDescription assignment;
	
	assignment = pD;

	EXPECT_EQ(assignment.project, pD.project);
}