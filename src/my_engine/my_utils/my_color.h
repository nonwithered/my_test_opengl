#pragma once

#include "my_utils/log.h"

inline std::array<float, 4> GetColorVec4(uint32_t argb) {
    float r = (argb >> 16) & 0xff;
    float g = (argb >> 8) & 0xff;
    float b = (argb >> 0) & 0xff;
    float a = (argb >> 24) & 0xff;
    return { r / 255, g / 255, b / 255, a / 255, };
}
