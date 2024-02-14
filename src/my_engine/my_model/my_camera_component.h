#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_cast.h"

#include "my_model/my_actor.h"
#include <my_model/my_mesh_component.h>

#include "my_graphic/my_uniform.h"

class CameraComponent : public Actor {

public: 

    static constexpr auto uniform_view = "view";
    static constexpr auto uniform_projection = "projection";

private:

    static constexpr auto TAG = "CameraComponent";

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

    void LookAt(UniformParameter &uniform) {
        uniform.emplace(uniform_view, UniformMatrix4fv::Make(false, { transform_view() }));
        uniform.emplace(uniform_projection, UniformMatrix4fv::Make(false, { transform_projection() }));
    }

    void DrawActor(Context &context, Actor &actor, const UniformParameter &uniform) {
        if (auto *mesh = TypeCast<MeshComponent>(&actor); mesh)  {
            mesh->Draw(context, uniform);
        }
        for (auto i = 0; i != actor.size(); ++i) {
            DrawActor(context, *actor.at(i), uniform);
        }
    }

protected:

    virtual glm::mat4 transform_projection() = 0;

public:

    virtual void Draw(Context &context, LevelActor &actor, const std::array<int, 4> &port) {
        auto [x, y, w, h] = port;
        glViewport(x, y, w, h);
        auto uniform = UniformParameter();
        LookAt(uniform);
        DrawActor(context, actor, uniform);
    }
};

class OrthoCameraComponent : public CameraComponent {

private:

    std::array<float, 2> vision_ = { 0.1, 1, };
    std::array<float, 4> sight_ = { -1, 1, -1, 1, };

protected:

    glm::mat4 transform_projection() final {
        return glm::ortho(sight_[0], sight_[1], sight_[2], sight_[3], vision_[0], vision_[1]);
    }

public:

    std::array<float, 2> vision() const {
        return vision_;
    }

    void vision(float near_, float far_) {
        vision_ = { near_, far_, };
    }

    std::array<float, 4> sight() const {
        return sight_;
    }

    void sight(float left, float right, float bottom, float top) {
        sight_ = { left, right, bottom, top, };
    }
};

class PerspectiveCameraComponent : public CameraComponent {

private:
    std::array<float, 2> vision_ = { 0.1, 1, };
    float sight_ = 45;
    float ratio_ = 1;

protected:

    glm::mat4 transform_projection() final {
        return glm::perspective(glm::radians(sight_), ratio_, vision_[0], vision_[1]);
    }

public:

    void Draw(Context &context, LevelActor &actor, const std::array<int, 4> &port) {
        auto [x, y, w, h] = port;
        ratio_ = (float) w / h;
        CameraComponent::Draw(context, actor, port);
    }

    std::array<float, 2> vision() const {
        return vision_;
    }

    void vision(float near_, float far_) {
        vision_ = { near_, far_, };
    }

    float sight() const {
        return sight_;
    }

    void sight(float v) {
        sight_ = v;
    }
};
