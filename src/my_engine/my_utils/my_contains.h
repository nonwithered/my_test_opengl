#pragma once

#include "my_utils/my_base.h"

template<typename T>
inline bool Contains(const std::vector<T> &vector, T e) {
    for (const auto &i : vector) {
        if (i == e) {
            return true;
        }
    }
    return false;
}
