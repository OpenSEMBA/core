#include "PhysicalModel.h"

namespace semba {
namespace PhysicalModel {

PhysicalModel::PhysicalModel(const std::string& name) 
{
    name_ = name;
}

PhysicalModel::PhysicalModel(const PhysicalModel& rhs) :
    Identifiable<Id>(rhs),
    name_( rhs.name_ )
{
}

const std::string& PhysicalModel::getName() const 
{
    return name_;
}
 
void PhysicalModel::setName(const std::string& newName) 
{
    name_ = newName;
}

} 
} 
