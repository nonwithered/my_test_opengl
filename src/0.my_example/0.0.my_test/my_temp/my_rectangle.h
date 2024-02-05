#pragma once

#include "my_resource/my_render.h"

#include "my_utils/my_color.h"

#include "my_utils/my_image.h"

class DrawRectangle {

private:

    static constexpr auto TAG = "RectangleTask";

    DrawRectangle(const DrawRectangle &) = delete;

    Shader shader_;
    VAO vao_;
    std::vector<Texture> textures_;
    std::function<void(UniformLocation)> uniform_;


public:

    DrawRectangle(DrawRectangle &&) = default;

    DrawRectangle(
        const std::string &vs,
        const std::string &fs,
        const std::vector<GLfloat> &data,
        const std::vector<GLuint> &configs,
        const std::vector<GLuint> &indices,
        const std::vector<std::string> &files,
        std::function<void(UniformLocation)> uniform)
    : shader_(vs, fs)
    , vao_(data, configs, indices)
    , uniform_(std::move(uniform)) {
        for (auto &file : files) {
            ImageData image(file);
            textures_.push_back(Texture(image.data(), image.width(), image.height(), image.type()));
        }
    }

    void operator()(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
        std::vector<Texture *> textures;
        for (auto &texture : textures_) {
            textures.push_back(&texture);
        }
        RenderDraw(shader_, vao_, textures, [&](UniformLocation location) {
            if (uniform_) {
                uniform_(location);
            }
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        });
    }
};

DrawRectangle DrawRectangleSingleColor(uint32_t color) {
    return DrawRectangle(
// vs
        R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
        )",
// fs
        R"(
#version 330 core
out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
    FragColor = ourColor;
}
        )",
// data
        {
// positions
            1.0f,   1.0f, 0.0f,  // top right
            1.0f,  -1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f,  // bottom left
            -1.0f,  1.0f, 0.0f,   // top left 
        },
// configs
        {
            3,
        },
// indices
        {  
            // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3,  // second Triangle
        },
// images
        {
        },
// uniform
        [color](UniformLocation location) {
            auto color_ = GetColorVec4(color);
            glUniform4f(location("ourColor"), color_[0], color_[1], color_[2], color_[3]);
        }
    );
}

DrawRectangle DrawRectangleMultiColor(std::array<uint32_t, 4> color) {
    auto right_top_color_ = GetColorVec4(color[0]);
    auto right_bottom_color_ = GetColorVec4(color[1]);
    auto left_bottom_color_ = GetColorVec4(color[2]);
    auto left_top_color_ = GetColorVec4(color[3]);
    return DrawRectangle(
// vs
        R"(
#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec4 aColor; // 颜色变量的属性位置值为 1

out vec4 ourColor; // 向片段着色器输出一个颜色

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}
        )",
// fs
        R"(
#version 330 core
out vec4 FragColor;  
in vec4 ourColor;

void main()
{
    FragColor = ourColor;
}
        )",
// data
        {
            // positions        // colors
            1.0f,   1.0f, 0.0f, right_top_color_[0],    right_top_color_[1],    right_top_color_[2],    right_top_color_[3],    // top right
            1.0f,  -1.0f, 0.0f, right_bottom_color_[0], right_bottom_color_[1], right_bottom_color_[2], right_bottom_color_[3], // bottom right
            -1.0f, -1.0f, 0.0f, left_bottom_color_[0],  left_bottom_color_[1],  left_bottom_color_[2],  left_bottom_color_[3],  // bottom left
            -1.0f,  1.0f, 0.0f, left_top_color_[0],     left_top_color_[1],     left_top_color_[2],     left_top_color_[3],     // top left 
        },
// configs
        {
            3, 4,
        },
// indices
        {
            // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3,  // second Triangle
        },
// images
        {
        },
// uniform
        nullptr
    );
}

DrawRectangle DrawRectangleWithPicture(const std::string &file) {
        return DrawRectangle(
// vs
        R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
        )",
// fs
        R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}
        )",
// data
        {
            // positions          // texture coords
            1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
        },
// configs
        {
            3, 2,
        },
// indices
        {
            // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3,  // second Triangle
        },
// images
        {
            file
        },
// uniform
        nullptr
    );
}
