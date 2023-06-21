#pragma once

#include <string>
#include "core/util/Identification.h"
#include "core/geometry/coordinate/Coordinate.h"
#include "core/util/Class.h"

namespace semba::geometry::junction {
	typedef geometry::CoordId CoordIdForJunctions;
	class Junction {

	public:
		static const std::string INVALID_ATTRIBUTE_MESSAGE;

		Junction(std::string & name);

		std::string const& getName() const;
		std::string getName();

		std::vector<std::vector<CoordIdForJunctions> > getUnitedCoordIds() const;

		void addCoordinatesToUnite(std::initializer_list<CoordIdForJunctions> coordinates);
		void addCoordinatesToUnite(const std::vector<int> & coordinates);

	private:
		std::string name;
		std::vector<std::vector<CoordIdForJunctions> > unitedCoordIds;
	};
}
