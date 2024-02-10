#pragma once

#include "my_utils/log.h"

class Sampler {

public:

    class Parameter {

        friend class Sampler;

    private:
        Parameter(Parameter &&) = delete;

        virtual void Tex(GLenum target, GLenum pname) const = 0;

    protected:
        Parameter() = default;
        Parameter(const Parameter &) = default;

    public:
        virtual ~Parameter() = default;

    };

    class Parameterf : public Parameter {

    private:
        using value_type = GLfloat;
        using self_type = Parameterf;

    public:
        static std::unique_ptr<Parameter> Make(value_type value) {
            return std::make_unique<self_type>(value);
        }

    private:
        value_type value_;

        void Tex(GLenum target, GLenum pname) const override {
            LOGD(TAG, "glTexParameterf %" PRIx64 " %" PRIx64 " %f", (uint64_t) target, (uint64_t) pname, value_);
            glTexParameterf(target, pname, value_);
        }

    public:

        Parameterf(value_type value) : value_(value) {
        }

    };

    class Parameteri : public Parameter {

        friend class Sampler;

    private:
        using value_type = GLint;
        using self_type = Parameteri;

    public:
        static std::unique_ptr<Parameter> Make(value_type value) {
            return std::make_unique<self_type>(value);
        }

    private:

        value_type value_;

        void Tex(GLenum target, GLenum pname) const override {
            LOGD(TAG, "glTexParameteri %" PRIx64 " %" PRIx64 " %" PRIx64, (uint64_t) target, (uint64_t) pname, (uint64_t) value_);
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
        using self_type = Parameterfv;

    public:
        static std::unique_ptr<Parameter> Make(vector_type value) {
            return std::make_unique<self_type>(value);
        }

        static std::unique_ptr<Parameter> Make(std::initializer_list<value_type> value) {
            return std::make_unique<self_type>(value);
        }

    private:

        vector_type value_;

        void Tex(GLenum target, GLenum pname) const override {
            LOGD(TAG, "glTexParameterfv %" PRIx64 " %" PRIx64 " %p", (uint64_t) target, (uint64_t) pname, value_.data());
            glTexParameterfv(target, pname, value_.data());
        }

    public:

        Parameterfv(const vector_type &value) : value_(value) {
        }

        Parameterfv(std::initializer_list<value_type> value) : value_(value) {
        }

    };

    class Parameteriv : public Parameter {

    private:
        using value_type = GLint;
        using vector_type = std::vector<value_type>;
        using self_type = Parameteriv;

    public:
        static std::unique_ptr<Parameter> Make(vector_type value) {
            return std::make_unique<self_type>(value);
        }

        static std::unique_ptr<Parameter> Make(std::initializer_list<value_type> value) {
            return std::make_unique<self_type>(value);
        }

    private:

        vector_type value_;

        void Tex(GLenum target, GLenum pname) const override {
            LOGD(TAG, "glTexParameteriv %" PRIx64 " %" PRIx64 " %p", (uint64_t) target, (uint64_t) pname, value_.data());
            glTexParameteriv(target, pname, value_.data());
        }

    public:

        Parameteriv(const vector_type &value) : value_(value) {
        }

        Parameteriv(std::initializer_list<value_type> value) : value_(value) {
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
        auto guard = Use();
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
                LOGD(TAG, "1D %x %d %x %d %x %x %p %d", target_, level, internalformat, border, format, type, pixels, width);
                glTexImage1D(target_, level, internalformat, width, border, format, type, pixels);
                break;
            }
            case GL_TEXTURE_2D: {
                auto width = size[0];
                auto height = size[1];
                LOGD(TAG, "2D %x %d %x %d %x %x %p %d %d", target_, level, internalformat, border, format, type, pixels, width, height);
                glTexImage2D(target_, level, internalformat, width, height, border, format, type, pixels);
                break;
            }
            case GL_TEXTURE_3D: {
                auto width = size[0];
                auto height = size[1];
                auto depth = size[2];
                LOGD(TAG, "3D %x %d %x %d %x %x %p %d %d %d", target_, level, internalformat, border, format, type, pixels, width, height, depth);
                glTexImage3D(target_, level, internalformat, width, height, depth, border, format, type, pixels);
                break;
            }
        }

        for (auto &[pname, param] : parameter) {
            switch (pname) {
                case GL_TEXTURE_MIN_FILTER:
                case GL_TEXTURE_MAG_FILTER: {
                    Parameteri *param_ = TypeCast<Parameteri>(param.get());
                    if (param_) {
                        switch (param_->value_) {
                            case GL_NEAREST_MIPMAP_NEAREST:
                            case GL_LINEAR_MIPMAP_NEAREST:
                            case GL_NEAREST_MIPMAP_LINEAR:
                            case GL_LINEAR_MIPMAP_LINEAR: {
                                LOGD(TAG, "glGenerateMipmap");
                                glGenerateMipmap(target_);
                            }
                        }
                    }
                }
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
        glGetTexParameterfv(target_, pname, params.data());
    }

    void GetTexParameteriv(GLenum pname, std::vector<GLint> &params) {
        Parameteriv parameter(params);
        glGetTexParameteriv(target_, pname, params.data());
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
