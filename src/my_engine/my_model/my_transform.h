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

    void translate(glm::vec3 translate) {
        translate_ = translate;
    }

    void rotate(glm::vec3 rotate) {
        rotate_ = rotate;
    }

    void scale(glm::vec3 scale) {
        scale_ = scale;
    }

    glm::vec3 translate() const {
        return translate_;
    }

    glm::vec3 rotate() const {
        return rotate_;
    }

    glm::vec3 scale() const {
        return scale_;
    }

    glm::mat4 matrix() const {
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, translate_);
        {
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
