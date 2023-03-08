#pragma once

#include "math/Types.h"
#include "math/Constants.h"

#include <iostream>
#include <typeinfo>

namespace SEMBA {
namespace Math {
namespace Function {

class Base {
public:
    Base() = default;
    virtual ~Base() = default;

    virtual Base* clone() const = 0;

    virtual bool operator==(const Base& rhs) const = 0;
};

template<class S, class T>
class Function : public Base {
public:
    Function() = default;
    virtual ~Function() = default;

    virtual T operator()(const S& arg) const = 0;
    T eval(const S& arg) const;
};

template<class S, class T>
inline T Function<S, T>::eval(const S& arg) const {
    return this->operator ()(arg);
}

} /* namespace Function */

typedef Function::Function<Real, Real> FunctionRR;

} /* namespace Math */
} /* namespace SEMBA */

#ifndef SEMBA_MATH_FUNCTION_DEFINE_CLONE
#define SEMBA_MATH_FUNCTION_DEFINE_CLONE(NAME)  \
    NAME* clone() const {                       \
        return new NAME(*this);                 \
    }
#endif

