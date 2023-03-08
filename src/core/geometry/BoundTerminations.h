#pragma once

#include <array>
#include <memory>
#include "geometry/element/Element.h"
#include "physicalModel/Bound.h"

namespace SEMBA {
namespace Geometry {

template <std::size_t D>
class BoundTerminations {
public:
    BoundTerminations() = default;
    BoundTerminations(const std::array<std::array<std::unique_ptr<PhysicalModel::Bound>, 2>, D>&);
    BoundTerminations(const BoundTerminations&);
    virtual ~BoundTerminations() = default;

    BoundTerminations& operator=(const BoundTerminations&);

    const PhysicalModel::Bound* operator()(std::size_t d, std::size_t p) const;

    void setBound (std::size_t d, std::size_t p, std::unique_ptr<PhysicalModel::Bound>&&);
    void setBounds(const std::array<std::array<std::unique_ptr<PhysicalModel::Bound>, 2>, D>&);

private:
    std::array<std::array<std::unique_ptr<PhysicalModel::Bound>, 2>, D> bounds_;
};

typedef BoundTerminations<3> BoundTerminations3;


template <std::size_t D>
BoundTerminations<D>::BoundTerminations(
    const std::array<std::array<std::unique_ptr<PhysicalModel::Bound>, 2>, D>& bounds) {
    setBounds(bounds);
}

template <std::size_t D>
BoundTerminations<D>::BoundTerminations(const BoundTerminations& rhs) {
    for (std::size_t d = 0; d < D; d++) {
        for (std::size_t p = 0; p < 2; p++) {
            if (rhs.bounds_[d][p]) {
                bounds_[d][p] = std::make_unique<PhysicalModel::Bound>(*(rhs.bounds_[d][p].get()));
            }
        }
    }
}

template <std::size_t D>
BoundTerminations<D>& BoundTerminations<D>::operator=(
    const BoundTerminations& rhs) {
    if (this == &rhs) {
        return *this;
    }

    for (std::size_t d = 0; d < D; d++) {
        for (std::size_t p = 0; p < 2; p++) {
            if (rhs.bounds_[d][p]) {
                bounds_[d][p] = std::make_unique<PhysicalModel::Bound>(*(rhs.bounds_[d][p].get()));
            }
        }
    }

    return *this;
}

template <std::size_t D>
const PhysicalModel::Bound* BoundTerminations<D>::operator()(std::size_t d,
    std::size_t p) const {
    if (bounds_[d][p]) {
        return bounds_[d][p].get();
    }

    throw std::logic_error("Could not retrieve Bound fot positions: " + std::to_string(d) + " and " + std::to_string(p));
}

template <std::size_t D>
void BoundTerminations<D>::setBound(
    std::size_t d,
    std::size_t p,
    std::unique_ptr<PhysicalModel::Bound>&& bound
) {
    bounds_[d][p] = std::move(bound);
}

template <std::size_t D>
void BoundTerminations<D>::setBounds(
    const std::array<std::array<std::unique_ptr<PhysicalModel::Bound>, 2>, D>& bounds
) {
    for (std::size_t d = 0; d < D; d++) {
        for (std::size_t p = 0; p < 2; p++) {
            if (bounds_[d][p]) {
                bounds_[d][p] = std::move(bound);
            }
        }
    }
}

} /* namespace Geometry */
} /* namespace SEMBA */

