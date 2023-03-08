#pragma once

#include <vector>
#include <set>

namespace SEMBA {
namespace Geometry {
namespace Graph {

template<class ELEM, class BOUND>
class Element {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef Element<Elem,Bound> GraphElem;
    typedef Element<Bound,Elem> GraphBound;

    Element(const Elem* elem, const std::size_t& numBounds = 0);
    Element(const Element&);

    Element* clone() const;

    bool visited() const { return vis_;  }
    void markVisited  () { vis_ =  true; }
    void unmarkVisited() { vis_ = false; }

    const Elem* elem() const { return elem_; }

    std::size_t numBounds() const { return bounds_.size(); }
    std::vector<const GraphBound*> getBounds() const;
    std::vector<GraphBound*>       getBounds()      { return bounds_;    }
    const GraphBound* getBound(std::size_t i) const { return bounds_[i]; }
    GraphBound*       getBound(std::size_t i)       { return bounds_[i]; }
    void setBounds(GraphBound* bound);
    void setBounds(std::vector<GraphBound*> bounds)  { bounds_ = bounds;   }
    void setBound (std::size_t i, GraphBound* bound) { bounds_[i] = bound; }
    void addBound (GraphBound* bound)                {
        bounds_.push_back(bound);
    }

    std::size_t numNeighbors() const { return neighbors_.size(); }
    const GraphElem* getNeighbor(std::size_t i) const { return neighbors_[i]; }
    GraphElem*       getNeighbor(std::size_t i)       { return neighbors_[i]; }

    std::size_t numBoundNeighbors(std::size_t i) const {
        return boundNeighbors_[i].size();
    }
    const GraphElem* getBoundNeighbor(std::size_t i, std::size_t j) const;
    GraphElem*       getBoundNeighbor(std::size_t i, std::size_t j);

    void constructNeighbors();

private:
    bool vis_;

    const Elem* elem_;
    std::vector<GraphBound*>             bounds_;
    std::vector<GraphElem*>              neighbors_;
    std::vector<std::vector<GraphElem*>> boundNeighbors_;
};


template<class ELEM, class BOUND>
Element<ELEM, BOUND>::Element(const ELEM* elem, const std::size_t& numBound) {
    vis_ = false;
    elem_ = elem;
    bounds_.resize(numBound);
}

template<class ELEM, class BOUND>
Element<ELEM, BOUND>::Element(const Element& rhs) {
    vis_ = rhs.vis_;
    elem_ = rhs.elem_;
    bounds_ = rhs.bounds_;
    neighbors_ = rhs.neighbors_;
    boundNeighbors_ = rhs.boundNeighbors_;
}

template<class ELEM, class BOUND>
Element<ELEM, BOUND>* Element<ELEM, BOUND>::clone() const {
    return new Element(*this);
}

template<class ELEM, class BOUND>
std::vector<const typename Element<ELEM, BOUND>::GraphBound*>
Element<ELEM, BOUND>::getBounds() const {
    return std::vector<const GraphBound*>(bounds_.begin(), bounds_.end());
}

template<class ELEM, class BOUND>
void Element<ELEM, BOUND>::setBounds(
    typename Element<ELEM, BOUND>::GraphBound* bound) {
    bounds_.resize(1);
    bounds_[0] = bound;
}

template<class ELEM, class BOUND>
const typename Element<ELEM, BOUND>::GraphElem*
Element<ELEM, BOUND>::getBoundNeighbor(std::size_t i, std::size_t j) const {
    return boundNeighbors_[i][j];
}

template<class ELEM, class BOUND>
typename Element<ELEM, BOUND>::GraphElem*
Element<ELEM, BOUND>::getBoundNeighbor(std::size_t i, std::size_t j) {
    return boundNeighbors_[i][j];
}

template<class ELEM, class BOUND>
void Element<ELEM, BOUND>::constructNeighbors() {
    boundNeighbors_.clear();
    boundNeighbors_.resize(bounds_.size());
    for (std::size_t i = 0; i < bounds_.size(); i++) {
        for (std::size_t j = 0; j < bounds_[i]->numBounds(); j++) {
            if (bounds_[i]->getBound(j)->elem()->getId() != elem_->getId()) {
                boundNeighbors_[i].push_back(bounds_[i]->getBound(j));
            }
        }
    }
    std::set<GraphElem*> neighbors;
    for (std::size_t i = 0; i < boundNeighbors_.size(); i++) {
        for (std::size_t j = 0; j < boundNeighbors_[i].size(); j++) {
            neighbors.insert(boundNeighbors_[i][j]);
        }
    }
    neighbors_ = std::vector<GraphElem*>(neighbors.begin(), neighbors.end());
}

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */

