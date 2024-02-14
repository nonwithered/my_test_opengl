#pragma once

#include "my_utils/my_log.h"

#include "my_model/my_actor.h"

#include "my_graphic/my_material.h"

class MeshComponent : public Actor {

public:

    static constexpr auto uniform_model = "model";

private:

    static constexpr auto TAG = "MeshComponent";

    std::shared_ptr<Mesh> mesh_;
    Material material_;

public:

    MeshComponent() = default;

    ~MeshComponent() = default;

    void mesh(std::shared_ptr<Mesh> mesh) {
        mesh_ = mesh;
    }

    Material &material() {
        return material_;
    }

    void Draw(Context &context, UniformParameter uniform) {
        if (!mesh_) {
            LOGD(TAG, "Draw mesh empty");
            return;
        }
        uniform.emplace(uniform_model, UniformMatrix4fv::Make(false, { transform_global() }));
        auto material = material_.Use(context, uniform);
        if (!material) {
            LOGD(TAG, "Draw mesh material");
            return;
        }
        auto &vertex = *context.resource().mesh().find(*mesh_);
        vertex.Draw();
    }

};
