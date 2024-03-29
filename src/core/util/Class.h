#pragma once

#include <cstddef>
#include <memory>

namespace semba {
namespace util {

class Class {
public:
    Class() {}
    virtual ~Class() {}

    template<typename T>
    bool is() const {
        if(dynamic_cast<const T*>(this) != nullptr) {
            return true;
        }
        return false;
    }

    template<typename T>
    T*       castTo() {
        return &dynamic_cast<T&>(*this);
    }
    template<typename T>
    const T* castTo() const {
        return &dynamic_cast<const T&>(*this);
    }
};

}
} 

