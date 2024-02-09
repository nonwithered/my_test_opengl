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
        auto Guard = Use();
        
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
