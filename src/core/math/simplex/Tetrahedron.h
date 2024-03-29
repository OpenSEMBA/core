#pragma once

#include "Simplex.h"
#include "Triangle.h"

namespace semba::math::simplex {

template <size_t N>
class Tetrahedron : public Simplex {
    friend class Triangle<N>;
public:
    static const std::size_t faces = 4;
    static const std::size_t n = N;
	static const std::size_t dimension = 3;
	static const std::size_t np = ((N + 1) * (N + 2) * (N + 3) / 6);
	static const std::size_t nfp = Triangle<N>::np;

    using MatIntNpNp = matrix::Static<Int, np, np>;
    using MatIntNfpNp = matrix::Static<Int, nfp, np>;
    
    static const std::size_t nsc = 4;
    using Index = CartesianVector<size_t,nsc>;

    static constexpr Real sizeFactor{ 1.0 / 6.0 };

    Tetrahedron();

    std::size_t vertex(const std::size_t) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t sideNode(const std::size_t f, const std::size_t i) const;

    std::size_t nodeIndex(const std::size_t node,
                          const std::size_t coordinate) const;

    const function::Polynomial<Real>& getLagr(const std::size_t i) const;
    const function::Polynomial<Real>& getDLagr(const std::size_t i, 
                                               const std::size_t f) const;
    std::vector<Real> getWeights() const;

    CVecR4 coordinate(const std::size_t i) const;
    CVecR4 sideCoordinate(const std::size_t f, const std::size_t i) const;

private:
    const Triangle<N> tri;

    std::array<Index,np> indices;
    CartesianVector<Real,nsc> nodePositions[np];
    std::array<Real,np> weights;

    function::Polynomial<Real> lagr[np];
    function::Polynomial<Real> dLagr[np][faces];

    matrix::Static<Int, faces, nfp> sNId;
    MatIntNfpNp RMatrix(const std::size_t s) const;
    MatIntNpNp PMatrix(const std::size_t s) const;

    static size_t numberOfNodes(size_t order);
};


template <size_t N>
Tetrahedron<N>::Tetrahedron() 
{
    matrix::Static<Int, np, nsc> ini;
    for (std::size_t i = 0; i <= N; i++) {
        for (std::size_t j = numberOfNodes(i - 1); j < np; j++) {
            ini(j, 0) = N - i;
        }
    }

    matrix::Static<Int, np, nsc> ord;
    for (std::size_t i = 0; i < nsc; i++) {
        ord = PMatrix(i) * ini;
        for (std::size_t j = 0; j < np; j++) {
            indices[j](i) = ord(j, 0);
        }
    }

    matrix::Static<Int, np, 1> nList;
    for (std::size_t i = 0; i < np; i++) {
        nList(i, 0) = i;
    }

    for (std::size_t f = 0; f < faces; f++) {
        matrix::Static<Int, nfp, 1> aux = RMatrix(f) * nList;
        for (std::size_t i = 0; i < nfp; i++) {
            sNId(f, i) = aux(i, 0);
        }
    }

    lagrangePolynomials(lagr, n, np, nsc);
    for (std::size_t i = 0; i < np; i++) {
        for (std::size_t s = 0; s < nsc; s++) {
            dLagr[i][s] = lagr[i];
            dLagr[i][s].derive(s);
        }
    }

    for (std::size_t i = 0; i < np; i++) {
        CartesianVector<Real, nsc> aux;
        nodePositions[i] = aux / (Real)N;
    }
    for (std::size_t i = 0; i < np; i++) {
        weights[i] = integrate(lagr[i], dimension, sizeFactor) / sizeFactor;
    }
}

template <size_t N>
inline std::vector<Real> Tetrahedron<N>::getWeights() const 
{
    std::vector<Real> res(np);
    std::copy_n(weights.begin(), np, res.begin());
    return res;
}

template <size_t N>
inline const function::Polynomial<Real>& Tetrahedron<N>::getLagr(
    const std::size_t i) const 
{
    return lagr[i];
}

template <size_t N>
inline const function::Polynomial<Real>& Tetrahedron<N>::getDLagr(
    const std::size_t i,
    const std::size_t f) const 
{
    return dLagr[i][f];
}

template <size_t N>
inline std::size_t Tetrahedron<N>::vertex(
    const std::size_t vertexNum) const 
{
    return sideNode(vertexNum, 0);
}

template <size_t N>
inline std::size_t Tetrahedron<N>::sideVertex(
    const std::size_t face,
    const std::size_t vertexNum) const 
{
    return sideNode(face, tri.vertex(vertexNum));
}

template <size_t N>
inline std::size_t Tetrahedron<N>::sideNode(
    const std::size_t face,
    const std::size_t num) const 
{
    return sNId(face, num);
}

template <size_t N>
inline std::size_t Tetrahedron<N>::nodeIndex(const std::size_t i,
    const std::size_t j) const 
{
    return indices[i](j);
}

template <size_t N>
CartesianVector<Real, 4> Tetrahedron<N>::coordinate(
    const std::size_t i) const 
{
    CartesianVector<Real, 4> res = indices[i];
    res /= (Real)n;
    return res;
}

template <size_t N>
CartesianVector<Real, 4> Tetrahedron<N>::sideCoordinate(
    const std::size_t f,
    const std::size_t i) const 
{
    return coordinate(sideNode(f, i));
}

template<size_t N>
size_t Tetrahedron<N>::numberOfNodes(size_t order) 
{
    size_t res = 1;
    for (size_t i = 1; i < size_t(nsc); i++) {
        res *= (order + i);
    }
    res /= factorial(nsc - 1);
    return res;
}

template <size_t N>
typename Tetrahedron<N>::MatIntNpNp
Tetrahedron<N>::PMatrix(std::size_t s) const {
    matrix::Static<Int, np, np> res;
    if (s == 0) {
        res.eye();
    }
    else if (s >= 1 && s <= 3) {
        // P2Dall will store the rotation matrix for all the indices slices.
        matrix::Dynamic<Int> P2Dall(np, np);
        std::size_t nodesSet = 0;
        for (std::size_t i = 0; i <= n; i++) {
            std::size_t sliceNP = (i + 1) * (i + 2) / 2;
            // Assign P2D to P2Dall matrix.
            matrix::Dynamic<Int> auxP = tri.PMatrix(i, 2);
            for (std::size_t j = 0; j < sliceNP; j++) {
                for (std::size_t k = 0; k < sliceNP; k++) {
                    P2Dall(j + nodesSet, k + nodesSet) = auxP.val(j, k);
                }
            }
            nodesSet += sliceNP;
        }
        // Allocates and initializes ordered and final indices vectors.
        matrix::Dynamic<Int> orVec(np, 1), fiVec(np, 1), temp(np, 1);
        for (std::size_t i = 0; i < np; i++) {
            orVec(i, 0) = i;
        }
        // Computes final indices positions.
        fiVec = P2Dall * orVec;
        // Assigns ones to the Q1 matrix.
        matrix::Dynamic<Int> Q[faces];
        matrix::Dynamic<Int> QInit(np, np);
        for (std::size_t i = 0; i < faces; i++) {
            Q[i] = QInit;
        }
        for (std::size_t i = 0; i < np; i++) {
            Q[0](orVec(i, 0), fiVec(i, 0)) = 1;
        }

        // Defines Q[1], same as Q[0] but around vertex 1.
        orVec.zeros();
        std::size_t lastSet = n * (n + 1) * (n + 2) / 6 + 1;
        std::size_t lastNodeSet = 0;
        for (std::size_t i = 0; i <= n; i++) {
            for (std::size_t j = 1; j <= i + 1; j++) {
                orVec(lastNodeSet + j - 1, 0) = lastSet;
                lastSet += 1;
                lastNodeSet += j;
            }
        }
        lastSet = n * (n + 1) * (n + 2) / 6 + 1;
        for (std::size_t j = 1; j <= n; j++) {
            temp = orVec;
            for (Int i = Int(np); i >= 2; i--) {
                if (temp(i - 1, 0) != 0) {
                    if (temp(i - 2, 0) == 0) {
                        lastSet--;
                        orVec(i - 2, 0) = lastSet;
                    }
                }
            }
        }

        fiVec = P2Dall * orVec;
        for (std::size_t i = 0; i < np; i++) {
            Q[1](orVec(i, 0) - 1, fiVec(i, 0) - 1) = 1;
        }
        matrix::Dynamic<Int> Q0Sq, Q1Sq;
        Q0Sq = Q[0] * Q[0];
        Q1Sq = Q[1] * Q[1];
        switch (s) {
        case 1:
            Q[2] = Q1Sq * Q0Sq; // Q[2] = Q[1] * Q[1] * Q[0] * Q[0];
            res = Q[2] * Q1Sq;  // res = Q[2] * Q[1] * Q[1];
            break;
        case 2:
            Q[2] = Q1Sq * Q0Sq; // Q[2] = Q[1] * Q[1] * Q[0] * Q[0];
            Q[3] = Q0Sq * Q1Sq; // Q[3] = Q[0] * Q[0] * Q[1] * Q[1];
            res = Q[3] * Q[2] * Q[2];
            break;
        case 3:
            Q[3] = Q0Sq * Q1Sq;// Q[3] = Q[0] * Q[0] * Q[1] * Q[1];
            res = Q[1] * Q[3] * Q[3];
            break;
        }
    }
    return res;
}

template <size_t N>
typename Tetrahedron<N>::MatIntNfpNp
Tetrahedron<N>::RMatrix(const std::size_t s) const 
{
    std::size_t last = 0;
    matrix::Static<Int, nfp, 1> nodeVec;
    for (std::size_t i = 1; i <= n + 1; i++) {
        std::size_t nsp = (i - 1) * (i) * (i + 1) / 6;
        for (std::size_t j = 0; j < i; j++) {
            nodeVec(last + j, 0) = nsp + (j) * (j + 1) / 2 + 1;
        }
        last += i;
    }

    matrix::Static<Int, nfp, np> Raux;
    for (std::size_t i = 0; i < nfp; i++) {
        Raux(i, nodeVec(i, 0) - 1) = 1;
    }

    return Raux * PMatrix(s);
}

}