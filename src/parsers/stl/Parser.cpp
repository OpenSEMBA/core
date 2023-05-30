#include "Parser.h"

#include "core/geometry/element/Triangle3.h"

namespace SEMBA {
namespace parsers {
namespace STL {

using namespace Geometry;
using namespace Math::Constants;

Parser::Parser(const std::string& fn) :
	SEMBA::parsers::Parser(fn)
{}

CoordR3Group readCoordinates(const std::string& fn)
{
	CoordR3Group cG;

    std::map<Math::CVecR3, const CoordR3*> index;


	std::ifstream stl(fn);
	std::string label;

	while (stl.peek() != EOF) {
		stl >> label;
		if (label != "vertex") {
			continue;
		}

		Math::CVecR3 pos;
		stl >> pos(x) >> pos(y) >> pos(z);

        auto it = index.find(pos);
        if (it == index.end()) {
            index.emplace(
                pos, 
                cG.addAndAssignId(
                    std::make_unique<CoordR3>(CoordId(), pos)
                )->get()
            );
        }
	}

	return cG;
}

std::pair<std::unique_ptr<Layer::Layer>, ElemRGroup> readLayerAndElements(
    const std::string& fn,
    const CoordR3Group& cG)
{
    std::unique_ptr<Layer::Layer> lay;
    ElemRGroup eG;

    auto cGIndex = cG.getIndex<Math::CVecR3>();
    
    std::ifstream stl(fn);
    stl.clear();
    while (stl.peek() != EOF) {
        std::string label;
        stl >> label;
        if (label != "solid") {
            continue;
        }

        std::string layerName;
        stl >> layerName;
        lay = std::make_unique<Layer::Layer>(layerName);
        std::string line;
        while (stl.peek() != EOF) {
            std::getline(stl, line);
            if (line.find("outer loop") == std::string::npos) {
                continue;
            }

            std::vector<const CoordR3*> coords;
            coords.reserve(3);
            while (stl.peek() != EOF && label != "endloop") {
                stl >> label;
                if (label != "vertex") {
                    continue;
                }

                Math::CVecR3 pos;
                stl >> pos(x) >> pos(y) >> pos(z);

                auto cIt = cGIndex.find(pos);
                if (cIt == cGIndex.end() || cIt->second.empty()) {
                    throw std::runtime_error("Unable to find coordinate during STL reading.");
                }

                coords.push_back(cIt->second.front());
            }

            label.clear();
            eG.addAndAssignId(
                std::make_unique<Tri3>(
                    Tri3(ElemId(0), coords.data(), lay.get())
                )
            );
        }
    }

    return std::make_pair(std::move(lay), eG);
}

Mesh::Unstructured Parser::readAsUnstructuredMesh() const 
{
    CoordR3Group cG = readCoordinates(this->filename);
    ElemRGroup eG;
    std::unique_ptr<Layer::Layer> lay;
    LayerGroup lG;
    std::tie(lay, eG) = readLayerAndElements(this->filename, cG);
    if (lay) {
        lG.addAndAssignId(std::move(lay));
    }

    return Mesh::Unstructured(cG, eG, lG);
}

} /* namespace STL */
} /* namespace Parser */
} /* namespace SEMBA */
