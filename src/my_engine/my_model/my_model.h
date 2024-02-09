#pragma once

#include "my_utils/log.h"

template<typename T>
class ModelGroup;

template<typename T>
class Model {

    friend class ModelGroup<T>;

private:
    using value_type = typename T;
    using self_type = typename Model<value_type>;

public:

    static std::shared_ptr<value_type> Make() {
        auto p = std::make_shared<value_type>();
        p->self(p);
        return p;
    }

private:

    static constexpr auto TAG = "Model";

    std::weak_ptr<value_type> self_;
    std::weak_ptr<value_type> parent_;

    Model(const self_type &) = delete;
    Model(self_type &&) = delete;

    void parent(std::shared_ptr<value_type> p) {
        if (parent_.lock() && p) {
            LOGE(TAG, "invalid parent");
            throw std::exception();
        }
        parent_ = p;
    }

public:

    Model() = default;

    virtual ~Model() = default;

    void self(const std::shared_ptr<value_type> &p) {
        if (p.get() != this) {
            LOGE(TAG, "invalid self");
            throw std::exception();
        }
        self_ = p;
    }

    std::shared_ptr<value_type> parent() {
        return parent_.lock();
    }
};

template<typename T>
class ModelGroup : public Model<T> {

private:
    using value_type = typename T;
    using self_type = typename ModelGroup<value_type>;

    std::vector<std::shared_ptr<value_type>> children_;

    ModelGroup(const self_type &) = delete;
    ModelGroup(self_type &&) = delete;

public:

    ModelGroup() = default;
    ~ModelGroup() = default;

    void insert(std::shared_ptr<value_type> child) {
        child->parent(self_.lock());
        children_.push_back(std::move(child));
    }

    void erase(std::shared_ptr<value_type> child) {
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

    std::shared_ptr<value_type> at(size_t index) {
        uint64_t index_ = index;
        uint64_t size_ = size();
        if (index_ >= size_) {
            LOGE(TAG, "invalid index %" PRIu64 " out of size %" PRIu64, index_, size_);
            throw std::exception();
        }
        return children_[index];
    }

};
