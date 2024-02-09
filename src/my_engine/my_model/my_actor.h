#pragma once

#include "my_utils/log.h"

#include "my_model/my_model.h"

#include "my_graphic/my_transform.h"

#include "my_manager/my_string_pool.h"

class Actor : public NestedModel<Actor> {

private:

    static constexpr auto TAG = "Actor";

    Actor(const Actor &) = delete;
    Actor(Actor &&) = delete;

    Transform transform_;

    glm::mat4 transform_global_ = glm::mat4();
    bool transform_global_cache_ = false;

    StringPool::Identify name_ = 0;

private:
    void TransformChanged() {
        {
            auto s = name();
            LOGD(TAG, "TransformChanged %s", s.data());
        }
        transform_global_cache_ = false;
        for (auto i = 0; i != size(); ++i) {
            at(i)->TransformChanged();
        }
    }

    void OnParentChanged() override {
        TransformChanged();
    }

public:

    Actor() = default;

    ~Actor() = default;

    const Transform &transform() const {
        return transform_;
    }

    void transform(const Transform &transform) {
        transform_ = transform;
        TransformChanged();
    }

    const glm::mat4 &transform_global() {
        if (!transform_global_cache_) {
            transform_global_cache_ = true;
            auto parent_ = parent();
            auto transform_local_ = transform_.matrix();
            if (!parent_) {
                transform_global_ = transform_local_;
            } else {
                transform_global_ = parent_->transform_global() * transform_local_;
            }
        }
        return transform_global_;
    }

    std::string name() const {
        return StringPool::Instance().Restore(name_);
    }

    void name(const std::string &name) {
        name_ = StringPool::Instance().Save(name);
    }
};
