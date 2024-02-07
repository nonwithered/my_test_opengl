#pragma once

#include "my_utils/log.h"

class Sampler {

private:

    static constexpr auto TAG = "Sampler";

    static GLuint NewId() {
        GLuint id;
        glGenTextures(1, &id);
        return id;
    }

    Sampler(const Sampler &) = delete;

    GLuint id_ = 0;

public:

    Sampler(const void *data, GLsizei width, GLsizei height, GLsizei type) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto scope = Use();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format;
        if (type == 3) {
            format = GL_RGB;
        } else if (type == 4) {
            format = GL_RGBA;
        } else {
            LOGE(TAG, "invalid image");
            throw std::exception();
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    Sampler(Sampler &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~Sampler() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteTextures(1, &id_);
        id_ = 0;
    }

    class Scope {

    private:

        static constexpr auto TAG = "Sampler.Scope";
        Scope(const Scope &) = delete;

        const Sampler *owner_;

    public:
        Scope(const Sampler &owner) : owner_(&owner) {
            LOGD(TAG, "bind %u", owner.id_);
            glBindTexture(GL_TEXTURE_2D, owner.id_);
        }
        Scope(Scope &&that) : owner_(that.owner_) {
            that.owner_ = nullptr;
        }
        ~Scope() {
            if (owner_ == nullptr) {
                return;
            }
            LOGD(TAG, "unbind");
            glBindTexture(GL_TEXTURE_2D, 0);
            owner_ = nullptr;
        }
    };

    Scope Use() const {
        return Scope(*this);
    }

};
