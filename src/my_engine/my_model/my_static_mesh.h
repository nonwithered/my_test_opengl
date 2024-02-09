#pragma once

#include "my_utils/log.h"

#include "my_actor.h"

#include "my_model/my_material.h"

class StaticMeshActor : public Actor {

private:

    static constexpr auto TAG = "StaticMeshActor";

    std::shared_ptr<Mesh> mesh_;
    Material material_;

public:

    StaticMeshActor() = default;

    ~StaticMeshActor() = default;

    void mesh(std::shared_ptr<Mesh> mesh) {
        mesh_ = mesh;
    }

    Material &material() {
        return material_;
    }

    void Draw(Context &context, const std::unordered_map<std::string, std::unique_ptr<Uniform_t>> &uniform) {
        if (!mesh_) {
            LOGD(TAG, "Draw mesh empty");
            return;
        }
        auto material = material_.Use(context, uniform);
        if (!material) {
            LOGD(TAG, "Draw mesh material");
            return;
        }
        auto &vertex = *context.resource().mesh().find(*mesh_);
        vertex.Draw();
    }

};
