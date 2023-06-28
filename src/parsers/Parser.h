#pragma once

#include <algorithm>

#include "core/ProblemDescription.h"

namespace SEMBA::parsers {
    bool strToBool(const std::string& value);
    bool toBool(const std::size_t param);

    std::string& trim(std::string& s);
    std::string& ltrim(std::string& s);
    std::string& rtrim(std::string& s);

class Parser {
public:
    Parser(const std::string& fn);  

protected:
    util::Project filename;
    static void postReadOperations(UnstructuredProblemDescription& res);
};

}