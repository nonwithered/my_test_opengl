#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_cast.h"

#include "my_model/my_actor.h"
#include "my_model/my_mesh_component.h"
#include "my_model/my_viewport_layout_component.h"

#include "my_graphic/my_uniform.h"

class CameraComponent : public Actor, public ViewportPresenter {

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

    void OnCreate() override {
        Actor::OnCreate();
        NewActor<ViewportLayoutComponent>();
    }

public:

    void Draw(Context &context, Actor &actor) {
        auto [x, y, w, h] = port();
        glViewport(x, y, w, h);
        auto uniform = UniformParameter();
        LookAt(uniform);
        DrawActor(context, actor, uniform);
    }
};

class OrthoCameraComponent : public CameraComponent {

private:

    using vision_t = std::array<float, 2>;
    PROPERTY(vision_t, vision);
    using sight_t = std::array<float, 4>;
    PROPERTY(sight_t, sight);

protected:

    glm::mat4 transform_projection() final {
        return glm::ortho(sight_[0], sight_[1], sight_[2], sight_[3], vision_[0], vision_[1]);
    }
};

class PerspectiveCameraComponent : public CameraComponent {

private:

    float ratio_ = 1.0f;

    using vision_t = std::array<float, 2>;
    PROPERTY(vision_t, vision);
    PROPERTY(float, sight);

protected:

    glm::mat4 transform_projection() final {
        return glm::perspective(glm::radians(sight_), ratio_, vision_[0], vision_[1]);
    }

public:
    
    void port(const std::array<int, 4> &v) override {
        CameraComponent::port(v);
        ratio_ = (float) std::get<2>(v) / std::get<3>(v);
    }
};
