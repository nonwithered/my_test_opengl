#pragma once

#include "my_utils/my_log.h"

#include "my_manager/my_cache.h"

#include "my_graphic/my_sampler.h"

using Texture = typename ResourceIdentify<Sampler>;

class ConstTexture : public Texture {

public:

    struct Parameter {
        const std::vector<GLsizei> &size;

        GLint level;
        GLint internalformat;
        GLint border;
        GLenum format;
        GLenum type;

        const void *pixels;

        const std::unordered_map<GLenum, std::unique_ptr<Sampler::Parameter>> &parameter;
    };

private:

    static constexpr auto TAG = "ConstTexture";

    const Parameter &parameter_;

public:

    ConstTexture(const Parameter &parameter) : Texture(), parameter_(parameter) {
    }

    std::unique_ptr<Sampler> Obtain() const override {
        return std::make_unique<Sampler>(
            parameter_.size,
            parameter_.level,
            parameter_.internalformat,
            parameter_.border,
            parameter_.format,
            parameter_.type,
            parameter_.pixels,
            parameter_.parameter
        );
    }

    std::unique_ptr<Texture> Clone() const override {
        return std::make_unique<ConstTexture>(parameter_);
    }

    uint64_t Hash() const override {
        return (uint64_t) &parameter_;
    }

    bool Equal(const Texture &that) const override {
        auto *that_ = TypeCast<ConstTexture>(&that);
        return that_ != nullptr && &parameter_ == &that_->parameter_;
    }

};
