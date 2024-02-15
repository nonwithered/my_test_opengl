#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_cast.h"
#include "my_utils/my_property.h"

#include "my_model/my_model.h"

#include "my_graphic/my_transform.h"

#include "my_manager/my_string_pool.h"

class Actor : public NestedModel<Actor> {

private:

    static constexpr auto TAG = "Actor";

    Actor(const Actor &) = delete;
    Actor(Actor &&) = delete;

    Transform transform_;

    StringPool::Identify name_ = 0;

    glm::mat4 transform_global_ = glm::mat4();
    bool transform_global_cache_ = false;

    glm::mat4 rotate_global_ = glm::mat4();
    bool rotate_global_cache_ = false;

private:
    void TransformChanged() {
        {
            auto s = name();
            LOGD(TAG, "TransformChanged %s", s.data());
        }
        transform_global_cache_ = false;
        rotate_global_cache_ = false;
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

    glm::mat4 transform_global() {
        if (!transform_global_cache_) {
            transform_global_cache_ = true;
            auto transform_local_ = transform_.matrix();
            auto parent_ = parent();
            if (!parent_) {
                transform_global_ = transform_local_;
            } else {
                transform_global_ = parent_->transform_global() * transform_local_;
            }
        }
        return transform_global_;
    }

    glm::mat4 rotate_global() {
        if (!rotate_global_cache_) {
            rotate_global_cache_ = true;
            auto rotate_local_ = transform_.rotate_transform(glm::mat4(1.0f));
            auto parent_ = parent();
            if (!parent_) {
                rotate_global_ = rotate_local_;
            } else {
                rotate_global_ = parent_->rotate_global() * rotate_local_;
            }
        }
        return rotate_global_;
    }

    std::string name() const {
        return StringPool::Instance().Restore(name_);
    }

    void name(const std::string &name) {
        name_ = StringPool::Instance().Save(name);
    }

    template<typename T>
    std::shared_ptr<T> Find(const std::string &name = "") {
        for (auto i = 0; i != size(); ++i) {
            auto p = at(i);
            auto t = TypeCast<T>(p);
            if (t && (name.empty() || t->name() == name)) {
                return std::dynamic_pointer_cast<T>(t->self().lock());
            }
        }
        return nullptr;
    }

    template<typename T>
    std::shared_ptr<T> LookUp(const std::string &name = "") {
        auto p = parent();
        while (p) {
            auto t = TypeCast<T>(p);
            if (t && (name.empty() || t->name() == name)) {
                return std::dynamic_pointer_cast<T>(t->self().lock());
            }
            p = p->parent();
        }
        return nullptr;
    }

    template<typename T>
    std::shared_ptr<T> NewActor(const std::string &name = "") {
        auto actor = Model<T>::Make();
        if (!name.empty()) {
            actor->name(name);
        }
        insert(actor);
        return actor;
    }
};
