#pragma once

#include <my_framework/my_runtime.h>

#include <my_model/my_mesh_component.h>

#include <my_utils/my_color.h>
#include <my_utils/my_image.h>

inline const ConstMesh::Parameter &RectPictureColorMeshParameter() {
    static const GLfloat data[] = {
            // positions          // texture coords
            1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    static const std::vector<std::pair<GLenum, GLsizei>> attrib = {
        std::make_pair(TypeOf<GLfloat>(), 3),
        std::make_pair(TypeOf<GLfloat>(), 2),
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
        sizeof(data) / sizeof(GLfloat) / 5,
        attrib,
        index_data,
        TypeOf<GLuint>(),
        6,
        elements,
    };
    return parameter;
}

inline const ConstShader::Parameter &RectPictureColorShaderParameter() {
    static const std::string vs = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
    )";
    static const std::string fs = R"(
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}
    )";
    static const ConstShader::Parameter parameter = {
        vs,
        fs,
    };
    return parameter;
}

inline const ConstTexture::Parameter &RectPictureColorTextureParameter() {
    static ImageData image("resources/textures/container.jpg");
    static const std::vector<GLsizei> size = { image.width(), image.height() };
    static const GLint level = 0;
    static const GLint internalformat = image.type() == 3 ? GL_RGB : image.type() == 4 ? GL_RGBA : throw std::exception();
    static const GLint border = 0;
    static const GLenum format = internalformat;
    static const GLenum type = TypeOf<GLubyte>();
    static const void *const pixels = image.data();
    static const auto params = []() -> auto {
        std::unordered_map<GLenum, std::unique_ptr<Sampler::Parameter>> params_;
        params_[GL_TEXTURE_WRAP_S] = Sampler::Parameteri::Make(GL_REPEAT);
        params_[GL_TEXTURE_WRAP_T] = Sampler::Parameteri::Make(GL_REPEAT);
        params_[GL_TEXTURE_MIN_FILTER] = Sampler::Parameteri::Make(GL_LINEAR_MIPMAP_LINEAR);
        params_[GL_TEXTURE_MAG_FILTER] = Sampler::Parameteri::Make(GL_LINEAR);
        return params_;
    }();
    static const ConstTexture::Parameter parameter = {
        size,
        level,
        internalformat,
        border,
        format,
        type,
        pixels,
        params,
    };
    return parameter;
}

class RectPictureColor : public MeshComponent {

private:

    static constexpr auto TAG = "RectPictureColor";

protected:

    void OnCreate() override {
        mesh(std::make_shared<ConstMesh>(RectPictureColorMeshParameter()));
        material().Shader(std::make_shared<ConstShader>(RectPictureColorShaderParameter()));
        material().Texture(0, std::make_shared<ConstTexture>(RectPictureColorTextureParameter()));
    }
};
