#pragma once

#include "my_utils/log.h"

#include "my_manager/my_cache.h"

#include "my_graphic/my_vertex.h"

using Mesh = typename ResourceIdentify<Vertex>;

class ConstMesh : public Mesh {

public:
    struct Parameter {

        const void *data;
        GLsizei count;
        const std::vector<std::pair<GLenum, GLsizei>> &attrib;

        const void *index_data;
        GLenum index_type;
        GLsizei index_count;

        const std::vector<std::pair<GLenum, GLsizei>> &elements;
    };

private:

    static constexpr auto TAG = "ConstMesh";

    const Parameter &parameter_;

public:

    ConstMesh(const Parameter &parameter) : Mesh(), parameter_(parameter) {
    }

    std::unique_ptr<Vertex> Obtain() const override {
        return std::make_unique<Vertex>(
            VertexArray(
                parameter_.data, parameter_.count, parameter_.attrib,
                parameter_.index_data, parameter_.index_type, parameter_.index_count
            ),
            parameter_.elements
        );
    }

    std::unique_ptr<Mesh> Clone() const override {
        return std::make_unique<ConstMesh>(parameter_);
    }

    uint64_t Hash() const override {
        return (uint64_t) &parameter_;
    }

    bool Equal(const Mesh &that) const override {
        auto *that_ = TypeCast<ConstMesh>(&that);
        return that_ != nullptr && &parameter_ == &that_->parameter_;
    }

};
