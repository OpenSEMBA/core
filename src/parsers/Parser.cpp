#include "Parser.h"

namespace SEMBA {
namespace Parsers {

Parser::Parser(const std::string& fn) :
    filename(fn)
{
    std::ifstream ifs(fn);
    if (!ifs.is_open()) {
        throw std::runtime_error("Unable to open file: " + fn);
    }
};

Math::CVecR3 Parser::strToCartesianVector(const std::string& str) {
    std::stringstream iss(str);
    std::string sub;
    Math::CVecR3 res;
    for (std::size_t i = 0; i < 3; i++) {
        iss >> sub;
        res(i) = atof(sub.c_str());
    }
    return res;
}

bool Parser::strToBool(const std::string& value) {
    if (atoi(value.c_str()) == 1) {
        return true;
    } else {
        return false;
    }
}

void Parser::postReadOperations(UnstructuredProblemDescription& res) const 
{
    if (res.analysis.find("geometryScalingFactor") != res.analysis.end()) {
        Math::Real scalingFactor{ 
            res.analysis.at("geometryScalingFactor").get<double>() };
        res.model.mesh.applyScalingFactor(scalingFactor);
        res.grids.applyScalingFactor(scalingFactor);
    }
}

} /* namespace Parser */
} /* namespace SEMBA */
