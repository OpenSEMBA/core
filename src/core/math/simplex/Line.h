

#pragma once

#include "math/matrix/Static.h"

#include "Simplex.h"

#include <algorithm>

namespace SEMBA {
namespace Math {
namespace Simplex {

template <size_t N>
class Line : public Simplex {
public:
    static const std::size_t faces = 2;
    static const std::size_t dimension = 1;
    static const std::size_t nsc = 2;
    static const std::size_t nfp = 1;
    static constexpr std::size_t np = (N + 1);
    static constexpr Real sizeFactor = 1.0;

    typedef Vector::Cartesian<size_t,nsc> Index;

    Line();
    std::size_t vertex(const std::size_t) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t sideNode( const std::size_t f, const std::size_t i) const;

    std::size_t nodeIndex(const std::size_t i, const std::size_t j) const;

    const Function::Polynomial<Real>& getLagr(
            const std::size_t node) const;
    const Function::Polynomial<Real>& getDLagr(
            const std::size_t node, const std::size_t simplex) const;

    std::vector<Real> getWeights() const;

private:
    std::array<Index,np> indices;
    Matrix::Static<Int,faces,nfp> sideNodes;

    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][faces];

    std::array<Vector::Cartesian<Real,nsc>, np> nodePositions;
    std::array<Real,np>                         weights;

    Matrix::Static<Int, 1, (N+1)> RMatrix(const std::size_t s) const;
    Matrix::Static<Int, (N+1), (N+1)> PMatrix(const std::size_t s) const;
};

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Line.hpp"

