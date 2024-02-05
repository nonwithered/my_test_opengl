#pragma once

#include "my_header/log.h"

class VertexShader {

private:

    static constexpr auto TAG = "VertexShader";

    static GLuint NewId() {
        GLuint id;
        id = glCreateShader(GL_VERTEX_SHADER);
        return id;
    }

    VertexShader(const VertexShader &) = delete;

    GLuint id_ = 0;

public:

    VertexShader(const std::string &src) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto src_ptr = src.data();
        glShaderSource(id_, 1, &src_ptr, nullptr);
        glCompileShader(id_);
        LOGI(TAG, "compile %u", id_);
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id_, 512, nullptr, infoLog);
            LOGE(TAG, "ctor fail %s", infoLog);
            throw std::exception();
        }
    }

    VertexShader(VertexShader &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~VertexShader() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteShader(id_);
        id_ = 0;
    }

    GLuint Id() const {
        return id_;
    }
};

class FragmentShader {

private:

    static constexpr auto TAG = "FragmentShader";

    static GLuint NewId() {
        GLuint id;
        id = glCreateShader(GL_FRAGMENT_SHADER);
        return id;
    }

    FragmentShader(const FragmentShader &) = delete;

    GLuint id_ = 0;

public:

    FragmentShader(const std::string &src) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto src_ptr = src.data();
        glShaderSource(id_, 1, &src_ptr, nullptr);
        glCompileShader(id_);
        LOGI(TAG, "compile %u", id_);
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id_, 512, nullptr, infoLog);
            LOGE(TAG, "ctor fail %s", infoLog);
            throw std::exception();
        }
    }

    FragmentShader(FragmentShader &&that) : id_(that.id_) {
        that.id_ = 0;
    }


    ~FragmentShader() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteShader(id_);
        id_ = 0;
    }

    GLuint Id() const {
        return id_;
    }
};

class ShaderProgram {

private:

    static constexpr auto TAG = "ShaderProgram";

    static GLuint NewId() {
        GLuint id;
        id = glCreateProgram();
        return id;
    }

    ShaderProgram(const ShaderProgram &) = delete;

    GLuint id_ = 0;

public:

    ShaderProgram(const VertexShader &vs_, const FragmentShader &fs_) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto &vs = vs_;
        auto &fs = fs_;
        glAttachShader(id_, vs.Id());
        glAttachShader(id_, fs.Id());
        glLinkProgram(id_);
        LOGI(TAG, "link %u VS %u FS %u", id_, vs.Id(), fs.Id());
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id_, 512, nullptr, infoLog);
            LOGE(TAG, "link fail %s", infoLog);
            throw std::exception();
        }
    }

    ShaderProgram(ShaderProgram &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~ShaderProgram() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteProgram(id_);
        id_ = 0;
    }

    class Scope {

    private:

        static constexpr auto TAG = "ShaderProgram.Scope";

        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

        GLuint id_;

    public:
        Scope(GLuint id) : id_(id) {
            LOGD(TAG, "bind %u", id_);
            glUseProgram(id_);
        }
        ~Scope() {
            if (!id_) {
                return;
            }
            id_ = 0;
            LOGD(TAG, "unbind");
            glUseProgram(0);
        }
    };

    Scope Use() {
        return Scope(id_);
    }
    
    GLint GetUniformLocation(const std::string &location) {
        return glGetUniformLocation(id_, location.data());
    }
};

class Shader {

private:

    static constexpr auto TAG = "Shader";
    ShaderProgram program_;

    template<typename V, int n, bool v, int m>
    struct Uniform {
        static constexpr auto uniform = nullptr;
    };

#define TEMPLATE_UNIFORM(V, n, T, t) \
    template<> \
    struct Uniform<V, n, false, 0> { \
        const PFNGLUNIFORM##n##T##PROC uniform = glUniform##n##t; \
    };

#define TEMPLATE_UNIFORM_V(V, n, T, t) \
    template<> \
    struct Uniform<V, n, true, 0> { \
        const PFNGLUNIFORM##n##T##VPROC uniform = glUniform##n##t##v; \
    };

#define TEMPLATE_UNIFORM_V_MATRIX(V, n, T, t) \
    template<> \
    struct Uniform<V, n, true, n> { \
        PFNGLUNIFORMMATRIX##n##T##VPROC uniform = glUniformMatrix##n##t##v; \
    }; 

#define TEMPLATE_UNIFORM_V_MATRIX_X(V, n, T, t, m) \
    template<> \
    struct Uniform<V, n, true, m> { \
        PFNGLUNIFORMMATRIX##n##X##m##T##VPROC uniform = glUniformMatrix##n##x##m##t##v; \
    };


#define TEMPLATE_UNIFORM_(V, T, t) \
    TEMPLATE_UNIFORM(V, 1, T, t) \
    TEMPLATE_UNIFORM(V, 2, T, t) \
    TEMPLATE_UNIFORM(V, 3, T, t) \
    TEMPLATE_UNIFORM(V, 4, T, t) \
    TEMPLATE_UNIFORM_V(V, 1, T, t) \
    TEMPLATE_UNIFORM_V(V, 2, T, t) \
    TEMPLATE_UNIFORM_V(V, 3, T, t) \
    TEMPLATE_UNIFORM_V(V, 4, T, t)

#define TEMPLATE_UNIFORM_MATRIX_(V, T, t) \
    TEMPLATE_UNIFORM_V_MATRIX(V, 2, T, t) \
    TEMPLATE_UNIFORM_V_MATRIX(V, 3, T, t) \
    TEMPLATE_UNIFORM_V_MATRIX(V, 4, T, t) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 2, T, t, 3) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 2, T, t, 4) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 3, T, t, 2) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 3, T, t, 4) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 4, T, t, 2) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 4, T, t, 3) \

TEMPLATE_UNIFORM_(GLfloat, F, f)
TEMPLATE_UNIFORM_(GLint, I, i)
TEMPLATE_UNIFORM_(GLuint, UI, ui)
TEMPLATE_UNIFORM_(GLdouble, D, d)

TEMPLATE_UNIFORM_MATRIX_(GLfloat, F, f)
TEMPLATE_UNIFORM_MATRIX_(GLdouble, D, d)

#undef TEMPLATE_UNIFORM
#undef TEMPLATE_UNIFORM_V
#undef TEMPLATE_UNIFORM_V_MATRIX
#undef TEMPLATE_UNIFORM_V_MATRIX_X
#undef TEMPLATE_UNIFORM_
#undef TEMPLATE_UNIFORM_MATRIX_

public:

    Shader(const Shader &) = default;
    Shader(Shader &&) = default;
    ~Shader() = default;

    Shader(ShaderProgram program) : program_(std::move(program)) {
    }

    template<typename T, int n>
    void Uniform(const std::string &location, T v...) {
        Uniform<T, n, false, 0>().uniform(program_.GetUniformLocation(location), v...);
    }

    template<typename T, int n>
    void Uniform(const std::string &location, GLsizei count, const T *value) {
        Uniform<T, n, true, 0>().uniform(program_.GetUniformLocation(location), count, value);
    }

    template<typename T, int n, int m>
    void Uniform(const std::string &location, GLsizei count, GLboolean transpose, const T *value) {
        Uniform<T, n, true, m>().uniform(program_.GetUniformLocation(location), count, transpose, value);
    }
};
