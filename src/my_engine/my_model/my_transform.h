#pragma once

#include "my_utils/log.h"

class Transform {

private:

    static constexpr auto TAG = "Transform";

    Transform(Transform &&) = delete;

    glm::vec3 translate_ = glm::vec3(0.0f);
    glm::vec3 rotate_ = direction_default();
    glm::vec3 scale_ = glm::vec3(1.0f);

public:

    Transform() = default;
    Transform(const Transform &) = default;
    ~Transform() = default;

    void operator=(const Transform &that) {
        this->~Transform();
        new (this) Transform(that);
    }

    const glm::vec3 &translate() const {
        return translate_;
    }

    const glm::vec3 &rotate() const {
        return rotate_;
    }

    const glm::vec3 &scale() const {
        return scale_;
    }

    glm::vec3 &translate() {
        return translate_;
    }

    glm::vec3 &rotate() {
        return rotate_;
    }

    glm::vec3 &scale() {
        return scale_;
    }

    glm::mat4 matrix() const {
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, translate_);
        if (glm::normalize(rotate_) != direction_default()) {
            auto [angle, axis] = direction_to_rotate(rotate_);
            transform = glm::rotate(transform, angle, axis);
        }
        transform = glm::scale(transform, scale_);
        return transform;
    }

private:
    static const glm::vec3 &direction_default() {
        static glm::vec3 direction_ = glm::vec3(1.0f, 0.0f, 0.0f);
        return direction_;
    }

    static std::pair<float, glm::vec3> direction_to_rotate(const glm::vec3 &direction_) {
        auto direction = glm::normalize(direction_);
        auto axis = glm::normalize(glm::cross(direction_default(), direction));
        auto angle = glm::angle(direction_default(), direction);
        return std::make_pair(angle, axis);
    }
};
