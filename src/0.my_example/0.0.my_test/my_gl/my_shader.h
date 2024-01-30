#pragma once

#include <log.h>

class VS {

private:

    static constexpr auto TAG = "VS";

    static GLuint NewId() {
        GLuint id;
        id = glCreateShader(GL_VERTEX_SHADER);
        return id;
    }

    VS(const VS &) = delete;

    GLuint id_ = 0;

public:

    VS(const std::string &src) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto src_ptr = src.data();
        glShaderSource(id_, 1, &src_ptr, nullptr);
        glCompileShader(id_);
        LOGI(TAG, "compile %u %s", id_, src.data());
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id_, 512, nullptr, infoLog);
            LOGE(TAG, "ctor fail %s", infoLog);
            throw std::exception();
        }
    }

    VS(VS &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    virtual ~VS() {
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

class FS {

private:

    static constexpr auto TAG = "FS";

    static GLuint NewId() {
        GLuint id;
        id = glCreateShader(GL_FRAGMENT_SHADER);
        return id;
    }

    FS(const FS &) = delete;

    GLuint id_ = 0;

public:

    FS(const std::string &src) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto src_ptr = src.data();
        glShaderSource(id_, 1, &src_ptr, nullptr);
        glCompileShader(id_);
        LOGI(TAG, "compile %u %s", id_, src.data());
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id_, 512, nullptr, infoLog);
            LOGE(TAG, "ctor fail %s", infoLog);
            throw std::exception();
        }
    }

    FS(FS &&that) : id_(that.id_) {
        that.id_ = 0;
    }


    virtual ~FS() {
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

public:

    Shader(const std::string &vs, const std::string &fs) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto vs_ = VS(vs);
        auto fs_ = FS(fs);
        glAttachShader(id_, vs_.Id());
        glAttachShader(id_, fs_.Id());
        glLinkProgram(id_);
        LOGI(TAG, "link %u VS %u FS %u", id_, vs_.Id(), fs_.Id());
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

    virtual ~Shader() {
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

    public:
        Scope(Shader &owner) {
            LOGD(TAG, "bind %u", owner.id_);
            glUseProgram(owner.id_);
        }
        ~Scope() {
            LOGD(TAG, "unbind");
            glUseProgram(0);
        }
    };

    Scope Use() {
        return Scope(*this);
    }
    
    GLint GetUniformLocation(const std::string &name) {
        return glGetUniformLocation(id_, name.data());
    }
};
