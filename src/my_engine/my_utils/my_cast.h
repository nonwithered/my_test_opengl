#pragma once

#include "my_utils/my_base.h"

template<typename T, typename P>
inline T *TypeCast(P *ptr) {
    return dynamic_cast<T *>(ptr);
}

template<typename T, typename P>
inline const T *TypeCast(const P *ptr) {
    return dynamic_cast<const T *>(ptr);
}

template<typename T, typename P>
inline std::shared_ptr<T> TypeCast(const std::shared_ptr<P> &ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

