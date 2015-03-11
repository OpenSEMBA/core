/*
 * Tet4.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET4_H_
#include "Tet4.h"
#endif

Tet4::Tet4() {

}

Tet4::Tet4(const CoordinateGroup<>& coordGr,
           const ElementId id,
           const CoordinateId vId[4],
           const LayerId layerId,
           const MatId   matId)
:   Tet(id, layerId, matId) {

    for (UInt i = 0; i < tet.np; i++) {
        const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Tet4::Tet4(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        if (!coord->is<CoordR3>()) {
            cerr << "ERROR @ Tet4::Tet4(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        v_[i] = coord->castTo<CoordR3>();
    }
    check();
}

Tet4::Tet4(const ElementId id,
           const CoordR3* v[4],
           const LayerId layerId,
           const MatId   matId)
:   Tet(id, layerId, matId) {

    for (UInt i = 0; i < tet.np; i++) {
        v_[i] = v[i];
    }
    check();
}

Tet4::Tet4(const Tet4& rhs)
:   Tet(rhs) {

    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tet4::~Tet4() {

}

ClassBase* Tet4::clone() const {
    return new Tet4(*this);
}

bool Tet4::isInnerPoint(const CVecR3& pos) const {
    if (!getBound().isInnerPoint(pos)) {
        return false;
    }
    // Checks if point is inside a tetrahedron using the following algorithm:
    // http://steve.hollasch.net/cgindex/geometry/ptintet.html
    StaMatrix<Real,4,4> mat;
    // Builds matrix D0.
    for (UInt i = 0; i < 4; i++) {
        for (UInt j = 0; j < 3; j++) {
            mat(i,j) = getVertex(i)->pos()(j);
        }
        mat(i,3) = (Real) 1.0;
    }
    Real det = mat.getDeterminant4x4();
    assert(det != 0);
    bool isPositive = (det > (Real) 0.0);
    // Checks rest of matrices. Last column is always 1.0.
    for (UInt k = 0; k < 4; k++) {
        // Copies pos in row k.
        for (UInt j = 0; j < 3; j++) {
            mat(k,j) = pos(j);
        }
        // Copies rest of vertices.
        for (UInt i = 0; i < 4; i++) {
            if (i != k) {
                for (UInt j = 0; j < 3; j++) {
                    mat(i,j) = getVertex(i)->pos()(j);
                }
            }
        }
        Real det = mat.getDeterminant4x4();
        if ((det > (Real) 0.0) != isPositive) {
            return false;
        }
    }
    return true;
}

bool Tet4::isCurvedFace(const UInt face) const {
    return false;
}

bool Tet4::isFaceContainedInPlane(
const UInt face,
const CartesianPlane plane) const {
    return getTri3Face(face)->isContainedInPlane(plane);
}

const CoordR3* Tet4::getSideV(const UInt f, const UInt i) const {
    return v_[tet.sideNode(f,i)];
}

const CoordR3* Tet4::getSideVertex(const UInt f, const UInt i) const {
    return v_[tet.sideVertex(f,i)];
}

Real Tet4::getVolume() const {
    StaMatrix<Real,3,3> mat;
    CVecR3 aux;
    for (UInt i = 1; i < 4; i++) {
        aux = getV(0)->pos() - getV(i)->pos();
        for (UInt j = 0; j < 3; j++) {
            mat(i-1,j) = aux(j);
        }
    }
    Real det = mat.getDeterminant3x3();
    return (det / ((Real) 6.0));
}

Real Tet4::getAreaOfFace(const UInt f) const {
    CVecR3 v1, v2;
    v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
    v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
    return ((Real) 0.5 * (v1 ^ v2).norm());
}

void Tet4::setV(const UInt i, const CoordR3* v) {
    v_[i] = v;
}

void Tet4::check() const {
    if(hasZeroVolume()) {
        cerr << "ERROR @ Tet4::check():"
             << "Element " << getId() << " has null volume." << endl;
    }
}

void Tet4::printInfo() const {
    cout << "--- Tet4 info ---" << endl;
    cout << "Id: " << getId() << endl;
    cout << "Coordinates:" << endl;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
        cout << endl;
    }
}

bool Tet4::hasZeroVolume() const {
    bool zeroVolume;
    CVecR3 initialVCoord, otherVCoord;
    initialVCoord = *v_[0];
    for (UInt d = 0; d < 3; d++) {
        zeroVolume = true;
        for (UInt i = 1; i < tet.np; i++) {
            otherVCoord = *v_[i];
            zeroVolume &= (initialVCoord(d) == otherVCoord(d));
        }
        if (zeroVolume) {
            return true;
        }
    }
    return false;
}
