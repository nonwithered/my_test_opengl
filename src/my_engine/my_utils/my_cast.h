#pragma once

template<typename T, typename P>
inline T *TypeCast(P *ptr) {
    return dynamic_cast<T *>(ptr);
}

template<typename T, typename P>
inline const T *TypeCast(const P *ptr) {
    return dynamic_cast<const T *>(ptr);
}

