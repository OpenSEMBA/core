// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#ifndef SEMBA_CORE_GROUP_PRINTABLE_H_
#define SEMBA_CORE_GROUP_PRINTABLE_H_

#include "Group.h"

namespace SEMBA {
namespace Group {

template<typename T>
class Printable : public virtual Base<T> {
public:
    Printable() {}
    virtual ~Printable() {}

    void printInfo() const;

};

} /* namespace Group */
} /* namespace SEMBA */

#include "Printable.hpp"

#endif /* SEMBA_CORE_GROUP_PRINTABLE_H_ */
