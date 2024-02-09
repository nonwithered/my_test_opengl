#pragma once

#include "my_utils/log.h"

#include "my_manager/my_cache.h"

#include "my_graphic/my_vertex.h"
#include "my_graphic/my_sampler.h"
#include "my_graphic/my_shader.h"

using Mesh = typename ResourceIdentify<Vertex>;
using Texture = typename ResourceIdentify<Sampler>;
using Shader = typename ResourceIdentify<ShaderProgram>;

class ResourceManager {

private:

    static constexpr auto TAG = "ResourceManager";

    ResourceManager(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;

    CacheManager<Mesh> mesh_;
    CacheManager<Texture> texture_;
    CacheManager<Shader> shader_;

public:

    ResourceManager() = default;
    ~ResourceManager() = default;

    CacheManager<Mesh> &mesh() {
        return mesh_;
    }

    CacheManager<Texture> &texture() {
        return texture_;
    }

    CacheManager<Shader> &shader() {
        return shader_;
    }
};
