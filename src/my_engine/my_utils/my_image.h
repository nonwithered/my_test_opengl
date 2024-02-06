#pragma once

#include <stb_image.h>

#include <learnopengl/filesystem.h>

#include "my_utils/log.h"

class ImageData {

private:

    static constexpr auto TAG = "ImageData";

    ImageData(const ImageData &) = delete;

    void *data_ = nullptr;

    GLsizei width_ = 0;
    GLsizei height_ = 0;

    GLsizei nrChannels_ = 0;

public:

    ImageData(const std::string &file) {
        auto path = FileSystem::getPath(file);
        data_ = stbi_load(path.data(), &width_, &height_, &nrChannels_, 0);
        if (!data_) {
            LOGE(TAG, "ctor fail  %s", file.data());
            throw std::exception();
        }
        LOGI(TAG, "ctor %s %d %d %d ", file.data(), width_, height_, nrChannels_);
    }

    ImageData(ImageData &&that) : data_(that.data_), width_(that.width_), height_(that.height_), nrChannels_(that.nrChannels_) {
        that.data_ = nullptr;
        that.width_ = 0;
        that.height_ = 0;
        that.nrChannels_ = 0;
    }

    ~ImageData() {
        if (data_ == nullptr) {
            return;
        }
        LOGI(TAG, "dtor");
        stbi_image_free(data_);
        data_ = nullptr;
        width_ = 0;
        height_ = 0;
        nrChannels_ = 0;
    }

    const void *data() const {
        return data_;
    }

    GLsizei width() const {
        return width_;
    }

    GLsizei height() const {
        return height_;
    }

    GLsizei type() const {
        return nrChannels_;
    }

};
