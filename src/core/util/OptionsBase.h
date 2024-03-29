#pragma once

#include <cstdlib>
#include <string>
#include <assert.h>

#include "core/math/CartesianVector.h"

#include "nlohmann/json.hpp"

namespace semba {
namespace util {

class OptionsBase {
public:
    typedef nlohmann::json json;

    static math::CVecR3 strToCVecR3(std::string str) {
        std::stringstream ss(str);
        math::CVecR3 res;
        ss >> res(math::Constants::x) >> res(math::Constants::y) >> res(math::Constants::z);
        return res;
    }

    static std::pair<math::CVecR3, math::CVecR3> strToBox(std::string str) {
        std::size_t begin = str.find_first_of("{");
        std::size_t end = str.find_last_of("}");
        std::string aux = str.substr(begin + 1, end - 1);
        std::stringstream iss(aux);
        math::CVecR3 max, min;
        for (std::size_t i = 0; i < 3; i++) {
            iss >> max(i);
        }
        for (std::size_t i = 0; i < 3; i++) {
            iss >> min(i);
        }
        return { min, max };
    }

    template <class T>
    static void setIfExists(const json& j, T& entry, std::string labelToCheck)
    {
        auto const it = j.find(labelToCheck);
        if (it != j.end()) {
            entry = it->get<T>();
        }
    }

    template <class T>
    static void setIfExistsUsingLabelConversion(
        const json& j,
        T& entry,
        std::string labelToCheck,
        std::function<T(std::string)> labelToClassFunction)
    {
        auto const it = j.find(labelToCheck);
        if (it != j.end()) {
            entry = labelToClassFunction(it->get<std::string>());
        }
    }

};

}
}