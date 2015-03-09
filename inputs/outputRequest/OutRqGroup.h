#ifndef OUTPUTREQUESTGROUP_H_
#define OUTPUTREQUESTGROUP_H_

#include "OutRq.h"
#include "OutRqFarField.h"

class OutRqGroup : public Group<OutRq> {
public:
    OutRqGroup();
	OutRqGroup(const vector<OutRq>&);
	uint countWithType(const Element::Type) const;
	const OutRq* getWithType(const uint i, const Element::Type type) const;
	void printInfo() const;
};

#	endif
