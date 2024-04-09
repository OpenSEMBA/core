
#include <gtest/gtest.h>

#include "core/geometry/element/Node.h"
#include "core/geometry/element/Line2.h"
#include "core/geometry/element/Hexahedron8.h"
#include "core/outputRequest/BulkCurrent.h"
#include "core/outputRequest/FarField.h"
#include "core/outputRequest/OnLine.h"
#include "core/outputRequest/OnPoint.h"
#include "core/outputRequest/OutputRequest.h"
#include "exporters/ExporterVTK.h"
#include <nlohmann/json.hpp>

using namespace semba::math::Constants;


using namespace std;
using namespace semba;
using namespace outputRequest;
using namespace geometry;

class ExporterVTKTest : public ::testing::Test {

    void SetUp(){
   
    }

    void TearDown() {}

protected:
    ExporterVTKTest() {
        resultFilesFolder_ = "./test/exporters/resultFiles/";
    }

    string resultFilesFolder_;
	string getCaseName(const string project) const {
		return resultFilesFolder_ + project;
	}

    string getVTKPath(const string project) {
        return getCaseName(project) + "/" + project + ".vtk";
    }

    string getVtu(const string project, const string name, OutputRequest * request) {
        return getVTKPath(project) + "/" + "OutRq_" + request->getTypeStrForName() + "_" + name + ".vtu";
    }

    void cleanFolders(const string path) const {
        if (filesystem::exists(path))
            filesystem::remove_all(path);
        
    }

    void createFolder(const string path) const {
        filesystem::create_directory(path);
    }
};

TEST_F(ExporterVTKTest, exportProbes)
{
    string projectName = "exportProbes";
    string bulkProbeName = "probe1";
    string pointProbeName = "probe2";
    string farFieldName = "probe3";
    string lineProbeName = "probe4";

    cleanFolders(getCaseName(projectName));
    createFolder(getCaseName(projectName));

    UnstructuredProblemDescription unstructured;

    map<string, double> dictionary;
    dictionary["geometryScalingFactor"] = 0.0;
    unstructured.analysis = nlohmann::json(dictionary);

    unstructured.model.mesh.coords().addAndAssignId(make_unique<CoordR3>(math::CVecI3(0, 0, 0)));
    unstructured.model.mesh.coords().addAndAssignId(make_unique<CoordR3>(math::CVecI3(1, 0, 0)));
    unstructured.model.mesh.coords().addAndAssignId(make_unique<CoordR3>(math::CVecI3(1, 1, 0)));
    unstructured.model.mesh.coords().addAndAssignId(make_unique<CoordR3>(math::CVecI3(0, 1, 0)));
    unstructured.model.mesh.coords().addAndAssignId(make_unique<CoordR3>(math::CVecI3(0, 0, 1)));
    unstructured.model.mesh.coords().addAndAssignId(make_unique<CoordR3>(math::CVecI3(1, 0, 1)));
    unstructured.model.mesh.coords().addAndAssignId(make_unique<CoordR3>(math::CVecI3(1, 1, 1)));
    unstructured.model.mesh.coords().addAndAssignId(make_unique<CoordR3>(math::CVecI3(0, 1, 1)));

    auto& bulkCoords = unstructured.model.mesh.coords().get();
    vector<CoordR3*> pointCoord{ unstructured.model.mesh.coords().atId(CoordId(1)) };
    auto& lineCoords{ unstructured.model.mesh.coords().atIds(vector<CoordId>{CoordId(2), CoordId(3)})};

    NodR node;
    node.setV(0, pointCoord[0]);

    unstructured.model.mesh.elems().addAndAssignId(make_unique<HexR8>(ElemId(1), bulkCoords));
    unstructured.model.mesh.elems().addAndAssignId(make_unique<NodR>(node));
    unstructured.model.mesh.elems().addAndAssignId(make_unique<LinR2>(ElemId(3), lineCoords));
    

    unstructured.outputRequests.addAndAssignId(make_unique<BulkCurrent>(Domain(), bulkProbeName, std::vector<ElemId>{ElemId(1)}, CartesianAxis::x, 5));
    unstructured.outputRequests.addAndAssignId(make_unique<OnPoint>(OutputRequest::Type::current, Domain(), pointProbeName, std::vector<ElemId>{ElemId(2)}));
    unstructured.outputRequests.addAndAssignId(make_unique<FarField>(Domain(), farFieldName, std::vector<ElemId>{ElemId(2)}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
    unstructured.outputRequests.addAndAssignId(make_unique<OnLine>(OutputRequest::Type::current, Domain(), lineProbeName, std::vector<ElemId>{ElemId(3)}));

    exporters::ExporterVTK exporter = exporters::ExporterVTK(unstructured, getCaseName(projectName) + "/" + projectName);
    
    EXPECT_TRUE(filesystem::exists(getVtu(projectName, bulkProbeName, unstructured.outputRequests.atId(Id(1)))));
    EXPECT_TRUE(filesystem::exists(getVtu(projectName, pointProbeName, unstructured.outputRequests.atId(Id(2)))));
    EXPECT_TRUE(filesystem::exists(getVtu(projectName, farFieldName, unstructured.outputRequests.atId(Id(3)))));
    EXPECT_TRUE(filesystem::exists(getVtu(projectName, lineProbeName, unstructured.outputRequests.atId(Id(4)))));
}
