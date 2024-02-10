#pragma once

#include "my_utils/log.h"

#include "my_model/my_actor.h"

#include "my_graphic/my_uniform.h"

class CameraActor : public Actor {

public: 

    static constexpr auto uniform_view = "view";
    static constexpr auto uniform_projection = "projection";

private:

    static constexpr auto TAG = "CameraActor";

    glm::vec3 direction_up_ = Transform::direction_up_default();

    glm::mat4 transform_view() {
        auto direction = direction_global();
        if (Transform::parallel(direction, direction_up_)) {
            {
                auto s = name();
                LOGD(TAG, "Gimbal Lock %s", s.data());
            }
            throw std::exception();
        }
        auto eye = position_global();
        auto center = eye + direction;
        auto up = direction_up_;
        return glm::lookAt(eye, center, up);
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

    glm::vec3 direction_up() const {
        return direction_up_;
    }

    void direction_up(const glm::vec3 &direction_up) {
        direction_up_ = direction_up;
    }
};
