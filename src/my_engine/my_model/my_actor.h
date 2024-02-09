#pragma once

#include "my_utils/log.h"

#include "my_model/my_model.h"

#include "my_model/my_transform.h"

class Actor : public ModelGroup<Actor> {

private:

    static constexpr auto TAG = "Actor";

    Actor(const Actor &) = delete;
    Actor(Actor &&) = delete;

    Transform transform_;

    // TODO
    glm::mat4 absolute_transform_matrix_cache_ = glm::mat4();
    bool absolute_transform_matrix_cached_ = false;

public:

    Actor() = default;

    ~Actor() = default;

    const Transform &transform() const {
        return transform_;
    }

    void transform(Transform transform) {
        transform_ = transform;
        absolute_transform_matrix_cached_ = false;
    }
};
