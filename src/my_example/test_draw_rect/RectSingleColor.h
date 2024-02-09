#pragma once

#include <my_framework/my_runtime.h>

#include <my_model/my_static_mesh.h>

#include <my_utils/my_color.h>

inline const ConstMesh::Parameter &RectSingleColorMeshParameter() {
    static const std::array<GLfloat, 12> data = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    static const std::vector<std::pair<GLenum, GLsizei>> attrib = {
        std::make_pair(TypeOf<GLfloat>(), 3),
    };
    static const std::array<GLuint, 6> index_data = {
        0, 1, 3,  // first Triangle
        1, 2, 3,  // second Triangle
    };
    static const std::vector<std::pair<GLenum, GLsizei>> elements = {
        std::make_pair(GL_TRIANGLES, 6),
    };
    static const ConstMesh::Parameter parameter = {
        data.data(),
        4,
        attrib,
        index_data.data(),
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

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
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

class RectSingleColor : public StaticMeshActor {

private:

    static constexpr auto TAG = "RectSingleColor";

    RectSingleColor() {
        mesh(std::make_shared<ConstMesh>(RectSingleColorMeshParameter()));
        material().Shader(std::make_shared<ConstShader>(RectSingleColorShaderParameter()));
        auto color = GetColorVec4(0xffffffff);
        material().Uniform("ourColor", Uniform4f(glm::fvec4(color[0], color[1], color[2], color[3])));
    }

public:

    static RectSingleColor &Instance() {
        static RectSingleColor instance_;
        return instance_;
    }
};
