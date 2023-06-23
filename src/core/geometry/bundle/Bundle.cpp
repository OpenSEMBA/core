#include "Bundle.h"

namespace semba::geometry::bundle {
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

	std::string const& Bundle::getName() const {
		return name;
	}

	std::string Bundle::getName() {
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