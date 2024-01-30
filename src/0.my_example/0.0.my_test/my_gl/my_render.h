#pragma once

#include <log.h>

#include "my_vertex.h"
#include "my_shader.h"
#include "my_texture.h"

using UniformLocation = std::function<int(std::string)>;

void Render(Shader &shader, VAO &vao, const std::vector<Texture *> &textures, std::function<void(UniformLocation)> draw) {
    static constexpr auto TAG = "Render";
    if (!draw) {
        LOGE(TAG, "invalid draw");
        throw std::exception();
    }
    auto shader_ = shader.Use();
    auto vao_ = vao.Use();
    std::vector<Texture::Scope> textures_;
    for (int i = 0; i != textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        textures_.push_back(textures[i]->Use());
    }
    draw([&](std::string name) -> int {
        auto location = shader.GetUniformLocation(name);
        LOGD(TAG, "uniform location %s %d", name.data(), location);
        return location;
    });
}
