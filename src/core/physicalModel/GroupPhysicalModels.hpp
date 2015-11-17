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
/*
 * GroupPhysicalModels.hpp
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */
#include "GroupPhysicalModels.h"

template<typename P>
GroupPhysicalModels<P>& GroupPhysicalModels<P>::operator=(VectorPtr<P>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<P, MatId>::operator=(rhs);
    return *this;
}

template<typename P>
GroupPhysicalModels<P>& GroupPhysicalModels<P>::operator=(VectorPtr<P>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<P, MatId>::operator=(std::move(rhs));
    return *this;
}

template<typename P>
void GroupPhysicalModels<P>::printInfo() const {
    cout << "---- GroupPhysicalModels info ----" << endl;
    cout << "Number of physical models: " << this->size() << endl;
    Group<P>::printInfo();
}
