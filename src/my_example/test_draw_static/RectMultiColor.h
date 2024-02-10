#pragma once

#include <my_framework/my_runtime.h>

#include <my_model/my_mesh_actor.h>

#include <my_utils/my_color.h>

inline const ConstMesh::Parameter &RectMultiColorMeshParameter() {
    static const auto right_top_color_ = GetColorVec4(0xffff0000);
    static const auto right_bottom_color_ = GetColorVec4(0xff00ff00);
    static const auto left_bottom_color_ = GetColorVec4(0xff0000ff);
    static const auto left_top_color_ = GetColorVec4(0xffffffff);
    static const GLfloat data[] = {
        // positions        // colors
        1.0f,   1.0f, 0.0f, right_top_color_[0],    right_top_color_[1],    right_top_color_[2],    right_top_color_[3],    // top right
        1.0f,  -1.0f, 0.0f, right_bottom_color_[0], right_bottom_color_[1], right_bottom_color_[2], right_bottom_color_[3], // bottom right
        -1.0f, -1.0f, 0.0f, left_bottom_color_[0],  left_bottom_color_[1],  left_bottom_color_[2],  left_bottom_color_[3],  // bottom left
        -1.0f,  1.0f, 0.0f, left_top_color_[0],     left_top_color_[1],     left_top_color_[2],     left_top_color_[3],     // top left 
    };
    static const std::vector<std::pair<GLenum, GLsizei>> attrib = {
        std::make_pair(TypeOf<GLfloat>(), 3),
        std::make_pair(TypeOf<GLfloat>(), 4),
    };
    static const GLuint index_data[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3,  // second Triangle
    };
    static const std::vector<std::pair<GLenum, GLsizei>> elements = {
        std::make_pair(GL_TRIANGLES, 6),
    };
    static const ConstMesh::Parameter parameter = {
        data,
        4,
        attrib,
        index_data,
        TypeOf<GLuint>(),
        6,
        elements,
    };
    return parameter;
}

inline const ConstShader::Parameter &RectMultiColorShaderParameter() {
    static const std::string vs = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}
    )";
    static const std::string fs = R"(
#version 330 core

out vec4 FragColor;  
in vec4 ourColor;

void main()
{
    FragColor = ourColor;
}
    )";
    static const ConstShader::Parameter parameter = {
        vs,
        fs,
    };
    return parameter;
}

class RectMultiColor : public MeshActor {

private:

    static constexpr auto TAG = "RectMultiColor";

protected:

    void OnCreate() override {
        mesh(std::make_shared<ConstMesh>(RectMultiColorMeshParameter()));
        material().Shader(std::make_shared<ConstShader>(RectMultiColorShaderParameter()));

        auto transform_ = transform();
        transform_.translate() = glm::vec3(0.5f, 0.5f, 0);
        transform_.scale() = glm::vec3(0.5f);
        transform(transform_);
    }
};
