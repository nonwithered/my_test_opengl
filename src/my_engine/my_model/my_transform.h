#pragma once

#include "my_utils/log.h"

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

    void Translate(float x, float y, float z) {
        translate_ += glm::vec3(x, y, z);
    }

    void Rotate(float x, float y, float z, float r) {
        rotate_ = glm::rotate(rotate_, glm::radians(r), glm::vec3(x, y, z));
    }

    void Scale(float x, float y, float z) {
        scale_ *= glm::vec3(x, y, z);
    }

    void translate(glm::vec3 translate) {
        translate_ = translate;
    }

    void rotate(glm::mat4 rotate) {
        rotate_ = rotate;
    }

    void scale(glm::vec3 scale) {
        scale_ = scale;
    }

    glm::vec3 translate() const {
        return translate_;
    }

    glm::mat4 rotate() const {
        return rotate_;
    }

    glm::vec3 scale() const {
        return scale_;
    }

    glm::mat4 matrix() const {
        auto transform = glm::mat4(1.0f);
        transform = glm::translate(transform, translate_);
        transform *= rotate_;
        transform = glm::scale(transform, scale_);
        return transform;
    }
};
