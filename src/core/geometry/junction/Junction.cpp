#include "Junction.h"
#include <stdexcept>

namespace semba::geometry::junction {
	const std::string Junction::INVALID_ATTRIBUTE_MESSAGE = "Error: There must be at least 2 coordinates to unite!";


	Junction::Junction(std::string & name)
	{
		this->name = name;
	}

	std::string const& Junction::getName() const
	{
		return name;
	}

	std::string Junction::getName()
	{
		return name;
	}

	std::vector<std::vector<CoordIdForJunctions> > Junction::getUnitedCoordIds() const
	{
		return unitedCoordIds;
	}

	void Junction::addCoordinatesToUnite(std::initializer_list<CoordIdForJunctions> coordinates)
	{
		if (coordinates.size() < 2)
			throw std::invalid_argument(Junction::INVALID_ATTRIBUTE_MESSAGE.c_str());

		unitedCoordIds.push_back(std::vector<CoordIdForJunctions>(coordinates));
	}

	void Junction::addCoordinatesToUnite(const std::vector<int> & coordinates)
	{
		if (coordinates.size() < 2)
			throw std::invalid_argument(Junction::INVALID_ATTRIBUTE_MESSAGE.c_str());

		unitedCoordIds.push_back(std::vector<CoordIdForJunctions>());

		for (int id : coordinates)
			unitedCoordIds.back().push_back(CoordIdForJunctions(id));
	}

}
