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

class Shader {

private:

    static constexpr auto TAG = "Shader";

    static GLuint NewId() {
        GLuint id;
        id = glCreateProgram();
        return id;
    }

    Shader(const Shader &) = delete;

    GLuint id_ = 0;
    
    GLint GetUniformLocation(const std::string &location) {
        return glGetUniformLocation(id_, location.data());
    }

public:

    Shader(const VertexShader &vs_, const FragmentShader &fs_) : id_(NewId()) {
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

    Shader(Shader &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~Shader() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteProgram(id_);
        id_ = 0;
    }

    class Scope {

    private:

        static constexpr auto TAG = "Shader.Scope";

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
