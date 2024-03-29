
#include "Simplex.h"

namespace semba {
namespace math {
namespace simplex {

Simplex::Simplex() {

}

Simplex::~Simplex() {

}

std::size_t Simplex::factorial(std::size_t n) {
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

function::Polynomial<Real> Simplex::silvesterPol(const std::size_t m,
                                                 const std::size_t n) const {
    // Purpose: Generates coefficients of the R polynomial as are defined in
    // Sylvester's book page 130. These polynomials have m equispace zeros to
    // the left of m/n and none to the right. These are necessary to build
    // Lagrange's polynomial.
    // Inputs:  Through arguments: Family member Int number "m". From Simplex
    // class: Simplex order "N".
    function::Polynomial<Real> res(1);
    res.addMonomial(1.0, 0);
    if (m != 0) {
        // Computes polynomial product.
        for (Int k = 0 ; k < Int(m); k++) {
            function::Polynomial<Real> aux(1);
            aux.addMonomial(Real(-k), 0);
            aux.addMonomial(Real(n), 1);
            aux.removeZeros();
            res *= aux;
        }
        // Computes factorial and divides by it.
        Real fact = 1.0;
        for (std::size_t k = 1; k <= m; k++) {
            fact *= k;
        }
        res /= fact;
    }
    return res;
}

void Simplex::lagrangePolynomials(function::Polynomial<Real>* res,
                                  const std::size_t n,
                                  const std::size_t np,
                                  const std::size_t nsc) const {
    // Computes Sylvester's polynomials.
    std::vector<function::Polynomial<Real>> pol(n+1);
    for (std::size_t i = 0; i < (n + 1); i++) {
        pol[i] = silvesterPol(i,n);
    }
    // Computes Lagrange's polynomials.
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t j = 0; j < nsc; j++) {
            if (j == 0) {
                res[i] = pol[nodeIndex(i,j)];
            } else {
                res[i] ^= pol[nodeIndex(i,j)];
            }
        }
    }
}

Real Simplex::integrate(const function::Polynomial<Real> pol,
                        const std::size_t dim,
                        const Real sizeFactor) const {
    assert(pol.numberOfVariables() == dim + 1);
    std::size_t nsc = dim + 1;
    Real sum = 0.0;
    Real auxNum;
    std::size_t auxDen;
    for (std::size_t i = 0; i < pol.numberOfMonomials(); i++) {
        auxNum = pol.monomialValue(i);
        auxDen = 0;
        for (std::size_t j = 0; j < nsc; j++) {
            auxNum *= factorial(pol.monomialPower(i,j));
            auxDen += pol.monomialPower(i,j);
        }
        sum += auxNum * factorial(dim) / factorial(auxDen + dim);
    }
    return (sizeFactor * sum);
}

} 
} 
} 
