#pragma once

#include <string>
#include "class/Identification.h"
#include "geometry/coordinate/Coordinate.h"
#include "class/Class.h"

namespace SEMBA::junction {
	typedef Geometry::CoordId CoordIdForJunctions;
	template<class T, std::size_t D>
	class Junction {

	public:
		Junction(std::string name)
		{
			this->name = name;
		}
		std::string getName()
		{
			return name;
		}
		std::vector<std::pair<CoordIdForJunctions, CoordIdForJunctions> > getUnitedCoordIds() 
		{
			return unitedCoordIds;
		}
		void addPairOfCoordinatesToUnite(CoordIdForJunctions firstCoordinate, CoordIdForJunctions secondCoordinate)
		{
			unitedCoordIds.push_back(std::pair<CoordIdForJunctions, CoordIdForJunctions>(firstCoordinate, secondCoordinate));
		}

	private:
		std::string name;
		std::vector<std::pair<CoordIdForJunctions, CoordIdForJunctions> > unitedCoordIds;
	};
}
