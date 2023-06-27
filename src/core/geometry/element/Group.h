#pragma once

#include "Element.h"
#include "Node.h"
#include "Line.h"
#include "Surface.h"
#include "Volume.h"

#include "core/util/GroupIdentifiableUnique.h"

namespace semba {
namespace geometry {
namespace element {

template<typename E = Elem>
class Group final : public util::GroupIdentifiableUnique<E> {
public:
    std::vector<const E*> atCoordId(const CoordId&) const;
	std::vector<const E*> atMatId(const MatId&) const;
    std::vector<const E*> atLayerId(const LayerId&) const ;
    std::vector<const E*> atMatLayerId(const MatId&, const LayerId&) const;
    
    BoxR3 getBound() const;
    
    template<class T>
    void reassignPointers(const coordinate::Group<coordinate::Coordinate<T,3>>& vNew);

    void reassignPointers(const LayerGroup& lNew);
    void reassignPointers(const PMGroup& mNew);
};


template<typename E>
std::vector<const E*> Group<E>::atCoordId(const CoordId& id) const
{
    std::vector<const E*> res;
    for (auto const& item : *this) {
        for (size_t j = 0; j < item->numberOfCoordinates(); j++) {
            if (item->getV(j)->getId() == id) {
                res.push_back(item.get());
            }
        }
    }
    return res;
}

template<typename E>
std::vector<const E*> Group<E>::atMatId(const MatId& id) const
{
    std::vector<const E*> res;
    for (auto const& item : *this) {
        if (item->getMatId() == id) {
            res.push_back(item.get());
        }
    }
    return res;
}

template<typename E>
std::vector<const E*> Group<E>::atLayerId(const LayerId& id) const
{
    std::vector<const E*> res;
    for (auto const& item : *this) {
        if (item->getLayerId() == id) {
            res.push_back(item.get());
        }
    }
    return res;
}

template<typename E>
std::vector<const E*> Group<E>::atMatLayerId(const MatId& mId, const LayerId& lId) const
{
    std::vector<const E*> res;
    for (auto const& item : *this) {
        if (item->getMatId() == mId && item->getLayerId() == lId) {
            res.push_back(item.get());
        }
    }
    return res;
}

template<typename E>
BoxR3 Group<E>::getBound() const
{
    if (this->size() == 0) {
        return BoxR3().setInfinity();
    }
    BoxR3 bound;
    for (auto const& elem : this->template getOf<ElemR>()) {
        bound << elem->getBound();
    }
    for (auto const& elem : this->template getOf<ElemI>()) {
        BoxI3 boxI = elem->getBound();
        math::CVecI3 minP = boxI.getMin();
        math::CVecI3 maxP = boxI.getMax();
        using math::CVecR3;
        using namespace math::Constants;
        bound << BoxR3(CVecR3(minP(x), minP(y), minP(z)), CVecR3(maxP(x), maxP(y), maxP(z)));
    }
    return bound;
}

template<typename E> template<class T>
void Group<E>::reassignPointers(const coordinate::Group< coordinate::Coordinate<T, 3> >& vNew)
{
    for (auto const& item : *this) {
        if (item->template is<Element<T>>()) {
            Element<T>* elem = item->template castTo<Element<T>>();
            for (std::size_t j = 0; j < elem->numberOfCoordinates(); j++) {
                CoordId vId = elem->getV(j)->getId();
                elem->setV(j, vNew.atId(vId));
            }
        }
    }
}

template<typename E>
void Group<E>::reassignPointers(const LayerGroup& lNew)
{
    for (auto const& item : *this) {
        if (item->getLayer() != nullptr) {
            item->setLayer(lNew.atId(item->getLayerId()));
        }
    }
}

template<typename E>
void Group<E>::reassignPointers(const PMGroup& mNew)
{
    for (auto const& item : *this) {
        if (item->getModel() != nullptr) {
            item->setModel(mNew.atId(item->getMatId()));
        }
    }
}

} 

typedef element::Group<Elem> ElemGroup;
typedef element::Group<ElemR> ElemRGroup;
typedef element::Group<ElemI> ElemIGroup;
typedef element::Group<SurfR> SurfRGroup;
typedef element::Group<VolR> VolRGroup;

} 
} 





