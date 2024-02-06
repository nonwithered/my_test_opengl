#pragma once

#include "my_utils/log.h"

class Transform {

private:

    static constexpr auto TAG = "Transform";

    Transform(Transform &&) = delete;

    std::array<float, 3> translate_ = { 0.0f, 0.0f, 0.0f };
    glm::mat4 rotate_ = glm::mat4(1.0f);
    std::array<float, 3> scale_ = { 1.0f, 1.0f, 1.0f };

public:

    Transform() = default;
    Transform(const Transform &) = default;

    void Translate(float x, float y, float z) {
        translate_[0] += x;
        translate_[1] += y;
        translate_[2] += z;
    }

    void Rotate(float x, float y, float z, float r) {
        rotate_ = glm::rotate(rotate_, glm::radians(r), glm::vec3(x, y, z));
    }

    void Scale(float x, float y, float z) {
        scale_[0] *= x;
        scale_[1] *= y;
        scale_[2] *= z;
    }

    glm::mat4 matrix() const {
        auto transform = glm::mat4(1.0f);
        {
            auto [x, y, z] = translate_;
            transform = glm::translate(transform, glm::vec3(x, y, z));
        }
        transform *= transform * rotate_;
        {
            auto [x, y, z] = scale_;
            transform = glm::scale(transform, glm::vec3(x, y, z));
        }
        return transform;
    }
};
