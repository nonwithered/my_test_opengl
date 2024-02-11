#pragma once

inline glm::fvec4 GetColorVec4(uint32_t argb) {
    float r = (float) ((argb >> 16) & 0xff);
    float g = (float) ((argb >> 8) & 0xff);
    float b = (float) ((argb >> 0) & 0xff);
    float a = (float) ((argb >> 24) & 0xff);
    return glm::fvec4(r / 255, g / 255, b / 255, a / 255);
}
