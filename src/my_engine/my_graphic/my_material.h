#pragma once

#include "my_utils/my_log.h"

#include "my_utils/my_defer.h"

#include "my_manager/my_resource_manager.h"
#include "my_manager/my_string_pool.h"

#include "my_framework/my_context.h"

class Material {

private:

    static constexpr auto TAG = "Material";

    Material(Material &&) = delete;

    std::shared_ptr<Shader> shader_;
    std::array<std::shared_ptr<Texture>, GL_ACTIVE_TEXTURE - GL_TEXTURE0> texture_;
    std::unordered_map<StringPool::Identify, std::unique_ptr<Uniform_t>> uniform_;

public:

    Material() = default;

    ~Material() = default;

    Material(const Material &that) : shader_(that.shader_), texture_(that.texture_) {
        for (auto &[k, v] : that.uniform_) {
            uniform_.emplace(k, std::unique_ptr<Uniform_t>(&v->Clone()));
        }
    }

    void operator=(const Material &that) {
        this->~Material();
        new (this) Material(that);
    }

    void Shader(std::shared_ptr<Shader> shader) {
        shader_ = std::move(shader);
    }

    void Texture(uint8_t i, std::shared_ptr<Texture> texture) {
        if (i >= texture_.size()) {
            LOGW(TAG, "texture invalid index %" PRIu8, i);
            return;
        }
        texture_[i] = std::move(texture);
    }

    void Uniform(const std::string &name) {
        auto name_ = StringPool::Instance().Save(name);
        uniform_.erase(name_);
    }

    void Uniform(const std::string &name, const Uniform_t &uniform) {
        auto name_ = StringPool::Instance().Save(name);
        auto i = uniform_.find(name_);
        auto v = std::unique_ptr<Uniform_t>(&uniform.Clone());
        if (i != uniform_.end()) {
            i->second = std::move(v);
            return;
        }
        uniform_.emplace(name_, std::move(v));
    }

    class Guard {

        friend class Material;

    private:

        static constexpr auto TAG = "Material.Guard";
        Guard(const Guard &) = delete;
        Guard(Guard &&) = delete;

        const ShaderProgram::Guard shader_;
        const std::vector<Sampler::Guard> texture_;


        Guard(ShaderProgram::Guard shader = ShaderProgram::Guard(), std::vector<Sampler::Guard> texture = std::vector<Sampler::Guard>())
        : shader_(std::move(shader))
        , texture_(std::move(texture)) {
        }

    public:
        ~Guard() = default;

        operator bool() const {
            return shader_.operator bool();
        }
    };

    Guard Use(Context &context, const UniformParameter &uniform) {
        if (!shader_) {
            return Guard();
        }
        auto &shader = *context.resource().shader().find(*shader_);
        auto shader_guard = shader.Use();
        std::vector<Sampler::Guard> texture_guard;
        for (auto &texture : texture_) {
            if (!texture) {
                continue;
            }
            texture_guard.push_back(context.resource().texture().find(*texture)->Use());
        }
        for (auto &[name, uniform_v] : uniform) {
            shader.Location(name).Uniform(*uniform_v);
        }
        for (auto &[name_, uniform_v] : uniform_) {
            auto name = StringPool::Instance().Restore(name_);
            shader.Location(name).Uniform(*uniform_v);
        }
        return Guard(std::move(shader_guard), std::move(texture_guard));
    }
};
