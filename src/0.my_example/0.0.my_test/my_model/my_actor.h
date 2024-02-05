#pragma once

#include "my_utils/log.h"

#include "my_model/my_transform.h"

class Actor {

public:

    static std::shared_ptr<Actor> Make() {
        auto p = std::make_shared<Actor>();
        p->self(p);
        return p;
    }

private:

    static constexpr auto TAG = "Actor";

    static const std::string &name_empty() {
        const std::string empty_ = "";
        return empty_;
    }

    std::weak_ptr<Actor> self_;
    std::weak_ptr<Actor> parent_;
    std::vector<std::shared_ptr<Actor>> children_;

    Transform transform_;
    std::string name_;

    Actor(Actor &&) = delete;

protected:
    Actor(const Actor &) = default;

    void self(const std::shared_ptr<Actor> &p) {
        self_ = p;
    }

public:
    Actor() = default;

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

    void insert(std::shared_ptr<Actor> child) {
        if (child->parent_.lock()) {
            LOGE(TAG, "invalid add child");
            throw std::exception();
        }
        child->parent_ = self_;
        children_.push_back(std::move(child));
    }

    void erase(std::shared_ptr<Actor> child) {
        for (auto i = children_.begin(); i != children_.end(); ) {
            if (*i == child) {
                children_.erase(i);
                return;
            }
        }
    }

    std::shared_ptr<Actor> Lookup(std::function<bool(Actor &)> block) {
        if (!block) {
            LOGE(TAG, "Lookup invalid %s", name().data());
            throw std::exception();
        }
        std::shared_ptr<Actor> p = self_.lock();
        while (p && !block(*p)) {
            p = p->parent_.lock();
        }
        return p;
    }

    bool Collect(const std::function<bool(Actor &)> &block) {
        if (!block) {
            LOGE(TAG, "Collect invalid %s", name().data());
            throw std::exception();
        }
        if (block(*this)) {
            return true;
        }
        for (auto &p : children_) {
            if(p->Collect(block)) {
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<Actor> Collect(const std::function<bool(Actor &, size_t)> &block) {
        for (auto i = 0; i != children_.size(); ++i) {
            auto &p = children_[i];
            if (block(*p, i)) {
                return p;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Actor> Transform(glm::mat4 &transform, const Actor *root = nullptr) {
        return Lookup([&transform, root](Actor &actor) -> bool {
            transform = actor.transform_.matrix() * transform;
            return actor.parent_.lock().get() == root;
        });
    }

    std::shared_ptr<Actor> Find(const std::string &s) {
        std::shared_ptr<Actor> p;
        Collect([&s, &p](Actor &actor) -> bool {
            bool b = actor.name() == s;
            if (b) {
                p = actor.self_.lock();
            }
            return b;
        });
    }

    const std::string &name() const {
        return name_;
    }

    void name(const std::string &s) {
        name_ = s;
    }

};
