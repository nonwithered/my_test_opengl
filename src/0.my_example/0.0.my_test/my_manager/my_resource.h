#pragma once

#include "my_utils/log.h"

#include "my_manager/my_cache.h"

#include "my_graphics/my_vertex.h"

using Mesh = ResourceIdentify<Vertex>;

class ResourceManager {

private:

    static constexpr auto TAG = "ResourceManager";

    ResourceManager(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;

    CacheManager<Mesh> mesh_;

public:

    ResourceManager() = default;
    ~ResourceManager() = default;

    CacheManager<Mesh> &mesh() {
        return mesh_;
    }
};
