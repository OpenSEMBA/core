#include "Bundle.h"

namespace semba::bundle {

Bundle::Bundle(
	const std::string& name,
	MatIdForBundle materialId,
	MatIdForBundle initialConnectorId,
	MatIdForBundle endConnectorId,
	const std::vector<ElemIdForBundle>& elemIds) :
	name(name),
	materialId(materialId),
	initialConnectorId(initialConnectorId),
	endConnectorId(endConnectorId),
	elemIds(elemIds) {}

const std::string & Bundle::getName() const {
	return name;
}

const MatIdForBundle Bundle::getMaterialId() const {
	return materialId;
}

const MatIdForBundle Bundle::getInitialConnectorId() const {
	return initialConnectorId;
}

const MatIdForBundle Bundle::getEndConnectorId() const {
	return endConnectorId;
}

std::vector<ElemIdForBundle> const& Bundle::getElemIds() const {
	return elemIds;
}

}