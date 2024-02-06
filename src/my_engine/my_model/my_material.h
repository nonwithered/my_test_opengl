#pragma once

#include "my_utils/log.h"

#include "my_manager/my_resource.h"

class Material {

private:

    static constexpr auto TAG = "Material";

    Material(Material &&) = delete;

    std::shared_ptr<Shader> shader_;
    std::array<std::shared_ptr<Texture>, 32> texture_;

public:

    Material() = default;
    Material(const Material &) = default;
    ~Material() = default;

    class Scope {

    private:

        static constexpr auto TAG = "Material.Scope";
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

    public:
        Scope() {
        }
        ~Scope() {
        }
    };

    Scope Use() {
        return Scope();
    }
};
