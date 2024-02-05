#pragma once

#include "my_header/log.h"

#include "my_resource/my_vertex.h"
#include "my_resource/my_shader.h"
#include "my_resource/my_texture.h"

void RenderDraw(Shader &shader, VAO &vao, const std::vector<Texture *> &textures, std::function<void(UniformLocation)> draw) {
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

class Render {

private:

    static constexpr auto TAG = "Render";

    std::shared_ptr<Shader> shader_;
    
    std::array<std::shared_ptr<Texture>, 32> textures_;

public:
    Render(const Render &) = default;
    Render(Render &&) = default;

    void Draw() {
        
    }

};
