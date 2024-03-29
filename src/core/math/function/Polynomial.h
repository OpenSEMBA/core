#pragma once

#include <vector>

#include "core/math/matrix/Dynamic.h"
#include "core/math/CartesianVector.h"

#include "Function.h"

namespace semba {
namespace math {
namespace function {

template<class T>
class Polynomial : public Function<T,T>{
public:
    Polynomial() = default;
    Polynomial(const std::size_t nvar);
    virtual ~Polynomial() = default;

    SEMBA_MATH_FUNCTION_DEFINE_CLONE(Polynomial);

    std::size_t numberOfVariables() const;
    std::size_t numberOfMonomials() const;

    T monomialValue(const std::size_t i) const;
    Int monomialPower(const std::size_t monomial,
                      const std::size_t variable) const;

    Polynomial<T>& operator=(const Polynomial<T> &param);

    bool operator==(const Base& rhs) const;

    // Product between polynomials assuming variables are the same.
    Polynomial<T>  operator* (const Polynomial<T> &param) const;
    Polynomial<T>& operator*=(const Polynomial<T> &param);

    // Product between polynomials assuming variables are different.
    Polynomial<T>  operator^ (const Polynomial<T> &param);
    Polynomial<T>& operator^=(const Polynomial<T> &param);

    Polynomial<T>& operator/=(const T param );
    void addMonomial(T val, std::vector<Int> pow);
    void addMonomial(T val, Int pow);
    Int maxPower() const;

    T operator()(const T&) const;
    T eval(const CartesianVector<T,1>& pos) const;
    T eval(const CartesianVector<T,2>& pos) const;
    T eval(const CartesianVector<T,3>& pos) const;
    T eval(const CartesianVector<T,4>& pos) const;

    void derive(std::size_t coord);

    Polynomial<T> vectorToPolynomial(T *v, std::size_t sv, std::size_t nvar);
    Polynomial<T> matrixToPolynomial(const matrix::Dynamic<T> &param) const;
    matrix::Dynamic<T> polynomialToMatrix() const;
    void removeZeros();

private:
    std::size_t nv_{0}; // Number of variables.
    std::size_t nm_{0}; // Number of monomials.
    std::vector<T> mv_; // Value of constant monomials coefficients.
    std::vector<std::vector<Int>> mp_; // nm x nv array, containing powers of monomials.
};


template<class T>
Polynomial<T>::Polynomial(const std::size_t nvar) {
    nv_ = nvar;
    nm_ = 0;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& param) {
    if (this == &param)
        return *this;
    nv_ = param.nv_;
    nm_ = param.nm_;
    mv_ = param.mv_;
    mp_ = param.mp_;
    return *this;
}

template<class T>
inline std::size_t Polynomial<T>::numberOfVariables() const {
    return nv_;
}

template<class T>
inline std::size_t Polynomial<T>::numberOfMonomials() const {
    return nm_;
}

template<class T>
inline T Polynomial<T>::monomialValue(const std::size_t i) const {
    return mv_[i];
}

template<class T>
inline Int Polynomial<T>::monomialPower(
    const std::size_t monomial,
    const std::size_t variable) const {
    return mp_[monomial][variable];
}

template<class T>
T Polynomial<T>::eval(const CartesianVector<T, 1>& pos) const {
    assert(1 == nv_);
    T prod;
    std::size_t m, v;
    T res = 0.0;
    for (m = 0; m < nm_; m++) {
        prod = 1.0;
        for (v = 0; v < nv_; v++)
            prod *= pow(pos(v), mp_[m][v]);
        res += mv_[m] * prod;
    }
    return res;
}

template<class T>
T Polynomial<T>::eval(const CartesianVector<T, 2>& pos) const {
    assert(2 == nv_);
    T prod;
    std::size_t m, v;
    T res = 0.0;
    for (m = 0; m < nm_; m++) {
        prod = 1.0;
        for (v = 0; v < nv_; v++)
            prod *= pow(pos(v), mp_[m][v]);
        res += mv_[m] * prod;
    }
    return res;
}

template<class T>
T Polynomial<T>::eval(const CartesianVector<T, 3>& pos) const {
    assert(3 == nv_);
    T prod;
    std::size_t m, v;
    T res = 0.0;
    for (m = 0; m < nm_; m++) {
        prod = 1.0;
        for (v = 0; v < nv_; v++)
            prod *= pow(pos(v), mp_[m][v]);
        res += mv_[m] * prod;
    }
    return res;
}

template<class T>
T Polynomial<T>::eval(const CartesianVector<T, 4>& pos) const {
    assert(4 == nv_);
    T prod;
    std::size_t m, v;
    T res = 0.0;
    for (m = 0; m < nm_; m++) {
        prod = 1.0;
        for (v = 0; v < nv_; v++)
            prod *= pow(pos(v), mp_[m][v]);
        res += mv_[m] * prod;
    }
    return res;
}

template<class T>
void Polynomial<T>::derive(std::size_t coord) {
    // Performs derivative with respect to coordinate coord.
    for (std::size_t m = 0; m < nm_; m++)
        if (mp_[m][coord] == 0)
            mv_[m] = 0.0;
        else {
            mv_[m] *= mp_[m][coord];
            mp_[m][coord]--;
        }
    // Removes monomials with value zero.
    removeZeros();
}

template<class T>
void Polynomial<T>::removeZeros() {
    for (std::size_t i = 0; i < nm_; i++)
        if (mv_[i] == 0) {
            nm_--;
            mv_.erase(mv_.begin() + i);
            mp_.erase(mp_.begin() + i);
        }
}

template<class T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T>& param) const {
    // PURPOSE:
    // Performs polynomial product.
    // Algebraically, convolution is the same operation as multiplying
    assert(nv_ == 1 && param.nv_ == 1);
    matrix::Dynamic<T> a, b, c;
    Polynomial<T> res(1);
    // Converts polynomials to vectors.
    a = polynomialToMatrix();
    b = param.polynomialToMatrix();
    // Performs product as vector convolution.
    c = a.convolute(b);
    // Copies result in this polynomial.
    res = matrixToPolynomial(c);
    // Returns result.
    return res;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial<T>& param) {
    assert(nv_ == 1 && param.nv_ == 1);
    matrix::Dynamic<T> a, b, c;
    // Converts polynomials to vectors.
    a = polynomialToMatrix();
    b = param.polynomialToMatrix();
    // Performs product as vector convolution.
    c = a.convolute(b);
    // Copies ;result in this polynomial.
    *this = matrixToPolynomial(c);
    // Returns result.
    return *this;
}

template<class T>
Polynomial<T> Polynomial<T>::operator^(const Polynomial<T>& param) {
    // PURPOSE: Performs external product between polynomials. This means that
    // variables in both polynomials will be treated as if they are different.
    std::size_t i, j;
    Polynomial<T> res(nv_ + param.nv_);
    std::vector<Int> pow;
    for (i = 0; i < nm_; i++)
        for (j = 0; j < param.nm_; j++) {
            pow = mp_[i];
            pow.insert(pow.end(), param.mp_[j].begin(), param.mp_[j].end());
            res.addMonomial(mv_[i] * param.mv_[j], pow);
        }
    return res;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator^=(const Polynomial<T>& param) {
    // PURPOSE: Performs external product between polynomials. This means that
    // variables in both polynomials will be treated as if they are different.
    std::size_t i, j;
    Polynomial<T> res(nv_ + param.nv_);
    std::vector<Int> pow;
    for (i = 0; i < nm_; i++)
        for (j = 0; j < param.nm_; j++) {
            pow = mp_[i];
            pow.insert(pow.end(), param.mp_[j].begin(), param.mp_[j].end());
            res.addMonomial(mv_[i] * param.mv_[j], pow);
        }
    *this = res;
    return *this;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator/=(const T param) {
    assert(param != 0);
    for (std::size_t i = 0; i < nm_; i++)
        mv_[i] /= param;
    return *this;
}

template<class T>
Polynomial<T> Polynomial<T>::vectorToPolynomial(T* v,
    std::size_t sv,
    std::size_t nvar) {
    assert(nvar == 1);
    Polynomial<T> res(1);
    for (Int i = 0; i < sv; i++)
        if (v[i] != 0)
            res.addMonomial(v[i], i);
    return res;
}

template<class T>
Polynomial<T> Polynomial<T>::matrixToPolynomial(
    const matrix::Dynamic<T>& param) const {
    assert(nv_ <= 2);
    Polynomial<T> res;
    if (param.nCols() == 1) {
        res.nv_ = 1;
        for (std::size_t i = 0; i < param.nRows(); i++)
            if (param(i, 0) != 0.0)
                res.addMonomial(param(i, 0), i);
    }
    else {
        std::vector<Int> pow(2, 0);
        res.nv_ = 2;
        for (std::size_t i = 0; i < param.nRows(); i++)
            for (std::size_t j = 0; j < param.nCols(); j++)
                if (param(i, j) != 0.0) {
                    pow[0] = i;
                    pow[1] = j;
                    res.addMonomial(param(i, j), pow);
                }
    }
    return res;
}

template<class T>
matrix::Dynamic<T> Polynomial<T>::polynomialToMatrix() const {
    assert(nv_ == 1);
    matrix::Dynamic<T>  res(maxPower() + 1, 1);
    // Copies monomials to vector positions.
    for (std::size_t i = 0; i < nm_; i++)
        res(mp_[i][0], 0) = mv_[i];
    return res;
}

template<class T>
void Polynomial<T>::addMonomial(T val, std::vector<Int> pow) {
    assert(nv_ == pow.size());
    nm_++;
    mv_.push_back(val);
    mp_.push_back(pow);
}

template<class T>
void Polynomial<T>::addMonomial(T val, Int pow) {
    assert(nv_ == 1);
    std::vector<Int> vPow(1, pow);
    nm_++;
    mv_.push_back(val);
    mp_.push_back(vPow);
}

template<class T>
Int Polynomial<T>::maxPower() const {
    // Returns maximum power present in this polynomial.
    std::size_t i, j;
    Int res = 0;
    for (i = 0; i < nv_; i++)
        for (j = 0; j < nm_; j++)
            if (mp_[j][i] > res)
                res = mp_[j][i];
    return res;
}

template<class T>
inline T Polynomial<T>::operator ()(const T& arg) const {
    return eval(CartesianVector<T, 1>(arg));
}

template<class T>
inline bool Polynomial<T>::operator ==(const Base& rhs) const {
    if (dynamic_cast<const Polynomial<T>*>(&rhs) == nullptr) {
        return false;
    }
    const Polynomial<T>* rhsPol = dynamic_cast<const Polynomial<T>*>(&rhs);
    bool areEqual = true;
    areEqual &= mv_ == rhsPol->mv_;
    areEqual &= mp_ == rhsPol->mp_;
    return areEqual;
}

} 
} 
} 


