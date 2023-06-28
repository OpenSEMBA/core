#include <gtest/gtest.h>

#include "core/geometry/element/Line2.h"
#include "core/physicalModel/Predefined.h"
#include "core/physicalModel/multiport/Predefined.h"
#include "core/physicalModel/wire/Wire.h"
#include "core/model/Model.h"

using ModelObject = semba::UnstructuredModel;

using namespace semba;
using namespace model;

TEST(ModelTest, CanCreate) {
	ModelObject model = ModelObject();

	EXPECT_NE(&model, nullptr);

	EXPECT_NE(&model.mesh, nullptr);
	EXPECT_TRUE(model.mesh.coords().empty());
	EXPECT_TRUE(model.mesh.elems().empty());
	EXPECT_TRUE(model.mesh.layers().empty());
	EXPECT_TRUE(model.bundles.empty());

	EXPECT_TRUE(model.physicalModels.empty());
}

TEST(ModelTest, CanInitializeGrid) {
	ModelObject model = ModelObject();

	CoordR3Group coordinatesGroup = CoordR3Group();
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			coordinate::Id(),
			math::CVecR3(0.0, 0.0, 0.0)
		)
	);
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			coordinate::Id(), 
			math::CVecR3(1.0, 2.0, 3.0)
		)
	);

	const CoordR3* coordinatesArgumentList[1] = {coordinatesGroup.atId(coordinate::Id(1))};
	ElemRGroup elementsGroup = ElemRGroup();
	elementsGroup.addAndAssignId(
		std::make_unique<NodR>(
			element::Id(), 
			coordinatesArgumentList
		)
	);

	mesh::Unstructured mesh(coordinatesGroup, elementsGroup);
	model.mesh = mesh;

	EXPECT_FALSE(model.mesh.coords().empty());
	EXPECT_EQ(2, model.mesh.coords().size());

	EXPECT_EQ(
		math::CVecR3(1.0, 2.0, 3.0), 
		(model.mesh.coords()).atId(coordinate::Id(2))->pos()
	);

	EXPECT_FALSE(model.mesh.elems().empty());
	EXPECT_EQ(1, model.mesh.elems().size());

	EXPECT_TRUE(model.mesh.layers().empty());
}

TEST(ModelTest, CanInitializePhysicalModels) {
	ModelObject model = ModelObject();

	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.addAndAssignId(
		std::make_unique<physicalModel::PEC>(
			physicalModel::Id(),
			"Material PEC"
		)
	);

	model.physicalModels = physicalModelsGroup;

	EXPECT_EQ(1, model.physicalModels.size());
	EXPECT_EQ(
		"Material PEC",
		model.physicalModels.atId(physicalModel::Id(1))->getName()
	);
}



TEST(ModelTest, CanInitializeBundles) {
	CoordR3Group coordinatesGroup = CoordR3Group();
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			coordinate::Id(),
			math::CVecR3(0.0, 0.0, 0.0)
		)
	);
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			coordinate::Id(),
			math::CVecR3(1.0, 2.0, 3.0)
		)
	);

	const CoordR3* coordinatesArgumentList[2] = { coordinatesGroup.atId(coordinate::Id(1)), coordinatesGroup.atId(coordinate::Id(2)) };
	ElemRGroup elementsGroup = ElemRGroup();
	elementsGroup.addAndAssignId(
		std::make_unique<LinR2>(
			element::Id(),
			coordinatesArgumentList
		)
	);
	mesh::Unstructured mesh(coordinatesGroup, elementsGroup);

	PMGroup physicalModelsGroup = PMGroup();

	physicalModelsGroup.addAndAssignId(
		std::make_unique<physicalModel::wire::Wire>(
			physicalModel::Id(),
			"Material Cable",
			0.0,
			0.0,
			0.0
		)
	);
	physicalModelsGroup.addAndAssignId(
		std::make_unique<physicalModel::multiport::Predefined>(
			physicalModel::Id(),
			"Connector Short",
			physicalModel::multiport::Multiport::Type::shortCircuit
		)
	);

	std::vector<bundle::Bundle> bundlesList = { bundle::Bundle(
		std::string("Cable"),
		physicalModelsGroup.get().at(0)->getId(),
		physicalModelsGroup.get().at(1)->getId(),
		physicalModelsGroup.get().at(1)->getId(),
		{elementsGroup.get().at(0)->getId()}
	) };

	ModelObject model(mesh, physicalModelsGroup, bundlesList);

	EXPECT_EQ(2, model.physicalModels.size());
	EXPECT_EQ(
		"Cable",
		model.bundles[0].getName()
	);
}

TEST(ModelTest, CanCopyConstructor) {
	CoordR3Group coordinatesGroup = CoordR3Group();
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			coordinate::Id(),
			math::CVecR3(0.0, 0.0, 0.0)
		)
	);
	coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			coordinate::Id(),
			math::CVecR3(1.0, 2.0, 3.0)
		)
	);

	const CoordR3* coordinatesArgumentList[2] = { coordinatesGroup.atId(coordinate::Id(1)), coordinatesGroup.atId(coordinate::Id(2)) };
	ElemRGroup elementsGroup = ElemRGroup();
	elementsGroup.addAndAssignId(
		std::make_unique<LinR2>(
			element::Id(),
			coordinatesArgumentList
		)
	);

	mesh::Unstructured mesh = mesh::Unstructured(coordinatesGroup, elementsGroup);

	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.addAndAssignId(
		std::make_unique<physicalModel::wire::Wire>(
			physicalModel::Id(),
			"Material Cable",
			0.0,
			0.0,
			0.0
		)
	);
	physicalModelsGroup.addAndAssignId(
		std::make_unique<physicalModel::multiport::Predefined>(
			physicalModel::Id(),
			"Connector Short",
			physicalModel::multiport::Multiport::Type::shortCircuit
		)
	);

	std::vector<bundle::Bundle> bundlesList = { bundle::Bundle(
		std::string("Cable"),
		physicalModelsGroup.get().at(0)->getId(),
		physicalModelsGroup.get().at(1)->getId(),
		physicalModelsGroup.get().at(1)->getId(),
		{elementsGroup.get().at(0)->getId()}
	) };

	ModelObject model(mesh, physicalModelsGroup, bundlesList);

	EXPECT_EQ(
		model.mesh.coords().atId(coordinate::Id(1)),
		model.mesh.elems().atId(ElemId(1))->getV(0)
	);
	EXPECT_EQ(
		model.mesh.coords().atId(coordinate::Id(2)),
		model.mesh.elems().atId(ElemId(1))->getV(1)
	);

	ModelObject newModel(model);

	ASSERT_EQ(2, newModel.physicalModels.size());
	ASSERT_EQ(2, model.physicalModels.size());
	EXPECT_NE(
		model.physicalModels.get().front(),
		newModel.physicalModels.get().front()
	);
	EXPECT_NE(
		model.physicalModels.get().back(),
		newModel.physicalModels.get().back()
	);

	EXPECT_EQ(
		model.physicalModels.get().front()->getName(),
		newModel.physicalModels.get().front()->getName()
	);

	EXPECT_EQ(
		model.physicalModels.get().back()->getName(),
		newModel.physicalModels.get().back()->getName()
	);

	auto newCoordinate1 = newModel.mesh.coords().atId(coordinate::Id(1));
	auto newCoordinate2 = newModel.mesh.coords().atId(coordinate::Id(2));
	EXPECT_EQ(
		newCoordinate1,
		newModel.mesh.elems().atId(ElemId(1))->getV(0)
	);
	EXPECT_EQ(
		newCoordinate2,
		newModel.mesh.elems().atId(ElemId(1))->getV(1)
	);

	auto coordinate1 = model.mesh.coords().atId(coordinate::Id(1));
	auto coordinate2 = model.mesh.coords().atId(coordinate::Id(2));
	EXPECT_NE(coordinate1, newCoordinate1);
	EXPECT_NE(coordinate2, newCoordinate2);

	EXPECT_EQ(coordinate1->pos(), newCoordinate1->pos());
	EXPECT_EQ(coordinate2->pos(), newCoordinate2->pos());

	ASSERT_EQ(1, newModel.bundles.size());
	ASSERT_EQ(1, model.bundles.size());

	EXPECT_NE(&(model.bundles.front()), &(newModel.bundles.front()));
	EXPECT_EQ(model.bundles.front().getName(), model.bundles.front().getName());

	auto& bundle = model.bundles.front();
	auto& newBundle = newModel.bundles.front();

	EXPECT_EQ(bundle.getMaterialId(), newBundle.getMaterialId());
	EXPECT_EQ(bundle.getInitialConnectorId(), newBundle.getInitialConnectorId());
	EXPECT_EQ(bundle.getEndConnectorId(), newBundle.getEndConnectorId());
	EXPECT_EQ(1, bundle.getElemIds().size());
	EXPECT_EQ(1, newBundle.getElemIds().size());
	EXPECT_EQ(bundle.getElemIds().front(), newBundle.getElemIds().front());


}

TEST(ModelTest, IsReassigningPhysicalGroupToMeshOnCopy) {
	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.add(
		std::make_unique<physicalModel::PEC>(
			physicalModel::Id(18),
			"Material PEC"
		)
	);

	CoordR3Group coordinatesGroup = CoordR3Group();
	auto it = coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			coordinate::Id(),
			math::CVecR3(0.0, 0.0, 0.0)
		)
	);

	const CoordR3* coordinatesArgumentList[1] = { it->get()};
	ElemRGroup elementsGroup = ElemRGroup();
	elementsGroup.addAndAssignId(
		std::make_unique<NodR>(
			element::Id(),
			coordinatesArgumentList,
			nullptr,
			physicalModelsGroup.get().front()
		)
	);

	ModelObject newModel = ModelObject();
	{
		ModelObject model(
			mesh::Unstructured(coordinatesGroup, elementsGroup),
			physicalModelsGroup,
			std::vector<bundle::Bundle>()
		);

		newModel = model;
	}

	EXPECT_FALSE(newModel.physicalModels.empty());
	EXPECT_EQ(
		newModel.mesh.elems().atId(ElemId(1))->getModel()->getId(),
		physicalModel::Id(18)
	);

}

TEST(ModelTest, IsReassigningPhysicalGroupToMeshOnConstruct) {
	PMGroup physicalModelsGroup = PMGroup();
	physicalModelsGroup.add(
		std::make_unique<physicalModel::PEC>(
			physicalModel::Id(18),
			"Material PEC"
		)
	);

	CoordR3Group coordinatesGroup = CoordR3Group();
	auto it = coordinatesGroup.addAndAssignId(
		std::make_unique<CoordR3>(
			coordinate::Id(),
			math::CVecR3(0.0, 0.0, 0.0)
		)
	);

	const CoordR3* coordinatesArgumentList[1] = { it->get()};
	ElemRGroup elementsGroup = ElemRGroup();
	elementsGroup.addAndAssignId(
		std::make_unique<NodR>(
			element::Id(),
			coordinatesArgumentList,
			nullptr,
			physicalModelsGroup.get().front()
		)
	);

	ModelObject model(
		mesh::Unstructured(coordinatesGroup, elementsGroup), 
		physicalModelsGroup,
		std::vector<bundle::Bundle>()
	);

	physicalModelsGroup = PMGroup();

	EXPECT_FALSE(model.physicalModels.empty());
	EXPECT_TRUE(physicalModelsGroup.empty());
	EXPECT_EQ(
		model.mesh.elems().atId(ElemId(1))->getModel()->getId(),
		physicalModel::Id(18)
	);
}
