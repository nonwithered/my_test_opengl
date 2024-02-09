#pragma once

#include "my_utils/log.h"

#include "my_manager/my_cache.h"

#include "my_graphic/my_shader_program.h"

using Shader = typename ResourceIdentify<ShaderProgram>;

class ConstShader : public Shader {

public:
    struct Parameter {
        
        const std::string &vs;
        const std::string &fs;
    };

private:

    static constexpr auto TAG = "ConstShader";

    const Parameter &parameter_;

public:
    ConstShader(const Parameter &parameter) : Shader(), parameter_(parameter) {
    }

    std::unique_ptr<ShaderProgram> Obtain() const override {
        return std::make_unique<ShaderProgram>(
            VertexShader(parameter_.vs),
            FragmentShader(parameter_.fs)
        );
    }

    std::unique_ptr<Shader> Clone() const override {
        return std::make_unique<ConstShader>(parameter_);
    }

    uint64_t Hash() const override {
        return (uint64_t) &parameter_;
    }

    bool Equal(const Shader &that) const override {
        auto *that_ = dynamic_cast<const ConstShader *>(&that);
        return that_ != nullptr && &parameter_ == &that_->parameter_;
    }

};
