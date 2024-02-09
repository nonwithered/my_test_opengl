#pragma once

#include "my_utils/log.h"

class Transform {

private:

    static constexpr auto TAG = "Transform";

    Transform(Transform &&) = delete;

    glm::vec3 translate_ = glm::vec3(0.0f);
    glm::vec3 rotate_ = glm::vec3(1.0f, 0.0f, 0.0f);
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
};
