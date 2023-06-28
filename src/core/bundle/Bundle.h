#pragma once

#include <string>
#include "core/util/Identification.h"
#include "core/geometry/element/Element.h"

namespace semba::bundle {

using ElemIdForBundle = geometry::ElemId;
using MatIdForBundle = MatId;

class Bundle {
public:
	Bundle(
		const std::string& name,
		MatIdForBundle materialId,
		MatIdForBundle initialConnectorId,
		MatIdForBundle endConnectorId,
		const std::vector<ElemIdForBundle>& elemIds);

	const std::string & getName() const;
	const MatIdForBundle getMaterialId() const;
	const MatIdForBundle getInitialConnectorId() const;
	const MatIdForBundle getEndConnectorId() const;
	std::vector<ElemIdForBundle> const& getElemIds() const;

private:
	std::string name;
	MatIdForBundle materialId;
	MatIdForBundle initialConnectorId;
	MatIdForBundle endConnectorId;
	std::vector<ElemIdForBundle > elemIds;
};

}