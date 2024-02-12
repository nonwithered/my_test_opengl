#pragma once

#include "my_utils/my_log.h"

#include "my_model/my_actor.h"

#include "my_graphic/my_uniform.h"

class CameraActor : public Actor {

public: 

    static constexpr auto uniform_view = "view";
    static constexpr auto uniform_projection = "projection";

private:

    static constexpr auto TAG = "CameraActor";

    glm::mat4 transform_view() {
        auto rotate_matrix = rotate_global();
        glm::vec3 direction = rotate_matrix * Transform::direction_default();
        auto eye = position_global();
        auto center = eye + direction;
        glm::vec3 up = rotate_matrix * Transform::direction_up_default();
        return glm::lookAt(eye, center, up);
    }

    glm::vec3 position_global() {
        return transform_global() * Transform::position_default();
    }

protected:

    virtual glm::mat4 transform_projection() = 0;

public:

    CameraActor() = default;

    ~CameraActor() = default;

    void LookAt(UniformParameter &uniform) {
        uniform.emplace(uniform_view, UniformMatrix4fv::Make(false, { transform_view() }));
        uniform.emplace(uniform_projection, UniformMatrix4fv::Make(false, { transform_projection() }));
    }
};
