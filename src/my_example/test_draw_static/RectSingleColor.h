#pragma once

#include <my_framework/my_runtime.h>

#include <my_model/my_mesh_actor.h>

#include <my_utils/my_color.h>

inline const ConstMesh::Parameter &RectSingleColorMeshParameter() {
    static const GLfloat data[] = {
        // positions
         1.0f,  1.0f, 0.0f,  // top right
         1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f   // top left
    };
    static const std::vector<std::pair<GLenum, GLsizei>> attrib = {
        std::make_pair(TypeOf<GLfloat>(), 3),
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

inline const ConstShader::Parameter &RectSingleColorShaderParameter() {
    static const std::string vs = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
    )";
    static const std::string fs = R"(
#version 330 core

out vec4 FragColor;

uniform vec4 ourColor;

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

class RectSingleColor : public MeshActor {

private:

    static constexpr auto TAG = "RectSingleColor";

protected:

    void OnCreate() override {
        mesh(std::make_shared<ConstMesh>(RectSingleColorMeshParameter()));
        material().Shader(std::make_shared<ConstShader>(RectSingleColorShaderParameter()));
        material().Uniform("ourColor", Uniform4f(GetColorVec4(0xffff0000)));

        auto transform_ = transform();
        transform_.translate() = glm::vec3(-0.5f, 0.5f, 0);
        transform_.scale() = glm::vec3(0.5f);
        transform(transform_);

        name(TAG);
    }
};
