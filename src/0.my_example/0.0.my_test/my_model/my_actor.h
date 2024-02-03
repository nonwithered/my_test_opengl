#pragma once

#include <log.h>

#include "my_model/my_transform.h"

class Actor {

private:

    static constexpr auto TAG = "Actor";

    Transform transform_;

    std::weak_ptr<Actor> self_;
    std::weak_ptr<Actor> parent_;
    std::vector<std::shared_ptr<Actor>> children_;

    Actor(Actor &&) = delete;

    std::shared_ptr<Actor> Lookup(std::function<bool(Actor &)> block) {
        if (!block) {
            LOGE(TAG, "Lookup invalid");
            throw std::exception();
        }
        std::shared_ptr<Actor> p = self_.lock();
        while (p && !block(*p)) {
            p = p->parent_.lock();
        }
        return p;
    }

protected:
    Actor() = default;
    Actor(const Actor &) = default;

public:

    virtual ~Actor() = default;

    void Translate(float x, float y, float z) {
        transform_.Translate(x, y, z);
    }

    void Rotate(float x, float y, float z, float r) {
        transform_.Rotate(x, y, z, r);
    }

    void Scale(float x, float y, float z) {
        transform_.Scale(x, y, z);
    }

    std::shared_ptr<Actor> Transform(glm::mat4 &transform, const Actor *root = nullptr) {
        return Lookup([&transform, root](Actor &actor) -> bool {
            transform = actor.transform_.operator glm::mat4() * transform;
            return actor.parent_.lock().get() == root;
        });
    }

    void operator+=(std::shared_ptr<Actor> child) {
        if (child->parent_.lock()) {
            LOGE(TAG, "invalid add child");
            throw std::exception();
        }
        child->parent_ = self_;
        children_.push_back(std::move(child));
    }

    void operator-=(std::shared_ptr<Actor> child) {
        for (auto i = children_.begin(); i != children_.end(); ) {
            if (*i == child) {
                children_.erase(i);
                return;
            }
        }
    }

};
