#pragma once

#include "my_utils/log.h"

class Model {

    friend class GroupModel;

public:

    template<typename T>
    static std::shared_ptr<T> Make() {
        auto p = std::make_shared<T>();
        p->self(p);
        return p;
    }

private:

    static constexpr auto TAG = "Model";

    std::weak_ptr<Model> self_;
    std::weak_ptr<Model> parent_;

    Model(const Model &) = delete;
    Model(Model &&) = delete;

    void parent(std::shared_ptr<Model> p) {
        if (parent_.lock() && p) {
            LOGE(TAG, "invalid parent");
            throw std::exception();
        }
        parent_ = p;
    }

public:

    Model() = default;

    virtual ~Model() = default;

    void self(const std::shared_ptr<Model> &p) {
        if (p.get() != this) {
            LOGE(TAG, "invalid self");
            throw std::exception();
        }
        self_ = p;
    }

    std::shared_ptr<Model> parent() {
        return parent_.lock();
    }
};

class GroupModel : public Model {

private:

    std::vector<std::shared_ptr<Model>> children_;

    GroupModel(const GroupModel &) = delete;
    GroupModel(GroupModel &&) = delete;

public:

    GroupModel() = default;
    ~GroupModel() = default;

    void insert(std::shared_ptr<Model> child) {
        child->parent(self_.lock());
        children_.push_back(std::move(child));
    }

    void erase(std::shared_ptr<Model> child) {
        if (!child) {
            LOGW(TAG,"erase child but nullptr");
            return;
        }
        for (auto i = children_.begin(); i != children_.end(); ) {
            if (*i == child) {
                children_.erase(i);
                return;
            }
        }
    }

    size_t size() const {
        return children_.size();
    }

    std::shared_ptr<Model> at(size_t index) {
        uint64_t index_ = index;
        uint64_t size_ = size();
        if (index_ >= size_) {
            LOGE(TAG, "invalid index %" PRIu64 " out of size %" PRIu64, index_, size_);
            throw std::exception();
        }
        return children_[index];
    }

};
