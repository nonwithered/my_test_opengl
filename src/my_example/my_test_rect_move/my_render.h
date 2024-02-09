#pragma once

#include <my_utils/log.h>

#include <my_graphics/my_vertex.h>
#include <my_graphics/my_shader.h>
#include <my_graphics/my_sampler.h>

/*

    Sampler(const void *data, GLsizei width, GLsizei height, GLsizei type) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto Guard = Use();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format;
        if (type == 3) {
            format = GL_RGB;
        } else if (type == 4) {
            format = GL_RGBA;
        } else {
            LOGE(TAG, "invalid image");
            throw std::exception();
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

*/

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
