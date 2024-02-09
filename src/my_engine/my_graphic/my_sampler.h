#pragma once

#include "my_utils/log.h"

class Sampler {

public:

    class Parameter {

        friend class Sampler;

    private:
        Parameter(Parameter &&) = delete;

        virtual void Tex(GLenum target, GLenum pname) const = 0;

        virtual void GetTex(GLenum target, GLenum pname) {
            LOGE(TAG, "GetTex unsupport");
            throw std::exception();
        }

    protected:
        Parameter() = default;
        Parameter(const Parameter &) = default;

    public:
        virtual ~Parameter() = default;

    };

    class Parameterf : public Parameter {

    private:
        using value_type = GLfloat;

        value_type value_;

        void Tex(GLenum target, GLenum pname) const override {
            glTexParameterf(target, pname, value_);
        }

    public:

        Parameterf(value_type value) : value_(value) {
        }

    };

    class Parameteri : public Parameter {

    private:
        using value_type = GLint;

        value_type value_;

        void Tex(GLenum target, GLenum pname) const override {
            glTexParameteri(target, pname, value_);
        }

    public:

        Parameteri(value_type value) : value_(value) {
        }

    };

    class Parameterfv : public Parameter {

    private:
        using value_type = GLfloat;
        using vector_type = std::vector<value_type>;

        vector_type &value_;

        void Tex(GLenum target, GLenum pname) const override {
            glTexParameterfv(target, pname, value_.data());
        }

        void GetTex(GLenum target, GLenum pname) override {
            glGetTexParameterfv(target, pname, value_.data());
        }

    public:

        Parameterfv(vector_type &value) : value_(value) {
        }

    };

    class Parameteriv : public Parameter {

    private:
        using value_type = GLint;
        using vector_type = std::vector<value_type>;

        vector_type &value_;

        void Tex(GLenum target, GLenum pname) const override {
            glTexParameteriv(target, pname, value_.data());
        }

        void GetTex(GLenum target, GLenum pname) override {
            glGetTexParameteriv(target, pname, value_.data());
        }

    public:

        Parameteriv(vector_type &value) : value_(value) {
        }

    };

private:

    static constexpr auto TAG = "Sampler";

    static GLuint NewId() {
        GLuint id;
        glGenTextures(1, &id);
        return id;
    }

    Sampler(const Sampler &) = delete;

    GLuint id_ = 0;

    GLenum target_ = 0;

public:

    Sampler(
        const std::vector<GLsizei> &size,
        GLint level, GLint internalformat,
        GLint border, GLenum format,
        GLenum type,
        const void *pixels,
        const std::unordered_map<GLenum, std::unique_ptr<Parameter>> &parameter
    ) {
        LOGI(TAG, "ctor %u", id_);
        switch (size.size()) {
            case 1: {
                target_ = GL_TEXTURE_1D;
                break;
            }
            case 2: {
                target_ = GL_TEXTURE_2D;
                break;
            }
            case 3: {
                target_ = GL_TEXTURE_3D;
                break;
            }
            default: {
                LOGE(TAG, "invalid target %" PRIu64, (uint64_t) size.size());
                throw std::exception();
            }
        }

        for (auto &[pname, param] : parameter) {
            param->Tex(target_, pname);
        }
        switch (target_) {
            case GL_TEXTURE_1D: {
                auto width = size[0];
                glTexImage1D(target_, level, internalformat, width, border, format, type, pixels);
                break;
            }
            case GL_TEXTURE_2D: {
                auto width = size[0];
                auto height = size[1];
                glTexImage2D(target_, level, internalformat, width, height, border, format, type, pixels);
                break;
            }
            case GL_TEXTURE_3D: {
                auto width = size[0];
                auto height = size[1];
                auto depth = size[2];
                glTexImage3D(target_, level, internalformat, width, height, depth, border, format, type, pixels);
                break;
            }
        }
    }

    Sampler(Sampler &&that) : id_(that.id_), target_(that.target_) {
        that.id_ = 0;
        that.target_ = 0;
    }

    ~Sampler() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteTextures(1, &id_);
        id_ = 0;
    }

    void GetTexParameterfv(GLenum pname, std::vector<GLfloat> &params) {
        Parameterfv parameter(params);
        ((Parameter &) parameter).GetTex(target_, pname);
    }

    void GetTexParameteriv(GLenum pname, std::vector<GLint> &params) {
        Parameteriv parameter(params);
        ((Parameter &) parameter).GetTex(target_, pname);
    }

    class Guard {

        friend class Sampler;

    private:

        static constexpr auto TAG = "Sampler.Guard";
        Guard(const Guard &) = delete;

        GLuint id_;

        Guard(GLuint id) : id_(id) {
            LOGD(TAG, "bind %u", id_);
            glBindTexture(GL_TEXTURE_2D, id_);
        }

    public:

        Guard(Guard &&that) : id_(that.id_) {
            id_ = 0;
        }

        ~Guard() {
            if (id_ == 0) {
                return;
            }
            LOGD(TAG, "unbind");
            glBindTexture(GL_TEXTURE_2D, 0);
            id_ = 0;
        }
    };

    Guard Use() const {
        return Guard(id_);
    }

};
