#pragma once

#include <string>
#include "core/util/Identification.h"
#include "core/geometry/coordinate/Coordinate.h"
#include "core/util/Class.h"

namespace semba::geometry::junction {

using CoordIdForJunctions = geometry::CoordId;

class Junction {

public:
	static const std::string & INVALID_ATTRIBUTE_MESSAGE;

	Junction(std::string & name);

	const std::string & getName() const;
	
	std::vector<std::vector<CoordIdForJunctions> > getUnitedCoordIds() const;

	void addCoordinatesToUnite(const std::initializer_list<CoordIdForJunctions> & coordinates);
	void addCoordinatesToUnite(const std::vector<int> & coordinates);

private:
	std::string name;
	std::vector<std::vector<CoordIdForJunctions> > unitedCoordIds;
};

}
