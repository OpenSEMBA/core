/*
 * Line.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#include "Line.h"

Line<void>::Line() {

}

Line<void>::~Line() {

}

template<class T>
Line<T>::Line() {

}

template<class T>
Line<T>::~Line() {

}

template class Line<Real>;
template class Line<Int >;
