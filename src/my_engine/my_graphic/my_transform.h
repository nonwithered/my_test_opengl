#pragma once

#include "my_utils/my_log.h"

class Transform {

private:

    static constexpr auto TAG = "Transform";

    Transform(Transform &&) = delete;

    glm::vec3 translate_ = glm::vec3(0.0f);
    glm::mat4 rotate_ = glm::mat4(1.0f);
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

    const glm::mat4 &rotate() const {
        return rotate_;
    }

    const glm::vec3 &scale() const {
        return scale_;
    }

    glm::vec3 &translate() {
        return translate_;
    }

    glm::mat4 &rotate() {
        return rotate_;
    }

    glm::vec3 &scale() {
        return scale_;
    }

    glm::mat4 translate_transform(const glm::mat4 &transform = glm::mat4(1.0f)) const {
        return glm::translate(transform, translate_);
    }

    glm::mat4 rotate_transform(const glm::mat4 &transform = glm::mat4(1.0f)) const {
        return transform * rotate_;
    }

    glm::mat4 scale_transform(const glm::mat4 &transform = glm::mat4(1.0f)) const {
        return glm::scale(transform, scale_);
    }

    glm::mat4 matrix() const {
        glm::mat4 transform(1.0f);
        transform = translate_transform(transform);
        transform = rotate_transform(transform);
        transform = scale_transform(transform);
        return transform;
    }

private:

    static constexpr auto PARALLEL_ANGLE = 1.0f / 360;

public:

    static glm::vec4 direction_default() {
        static auto direction_ = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        return direction_;
    }

    static glm::vec4 position_default() {
        static auto position_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        return position_;
    }

    static glm::vec4 direction_up_default() {
        static auto up_ = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        return up_;
    }

    static bool parallel(const glm::vec3 &lhs, const glm::vec3 &rhs) {
        auto angle = std::abs(glm::degrees(glm::angle(lhs, rhs)));
        auto mod = std::fmod(angle, 180.0f);
        return mod < PARALLEL_ANGLE;
    }

    static bool parallel_same(const glm::vec3 &lhs, const glm::vec3 &rhs) {
        auto angle = std::abs(glm::degrees(glm::angle(lhs, rhs)));
        auto mod = std::fmod(angle, 360.0f);
        return mod < PARALLEL_ANGLE;
    }

    static std::pair<float, glm::vec3> direction_to_rotate(const glm::vec3 &direction_) {
        auto direction = glm::normalize(direction_);
        auto axis = glm::normalize(glm::cross(glm::vec3(direction_default()), direction));
        auto angle = glm::angle(glm::vec3(direction_default()), direction);
        return std::make_pair(angle, axis);
    }
};
