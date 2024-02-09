#pragma once

#include "my_utils/log.h"

template<typename T>
class NestedModel;

template<typename T>
class Model {

private:
    using value_type = typename T;
    using self_type = typename Model<value_type>;

public:

    template<typename ...Args>
    static std::shared_ptr<value_type> Make(Args... args) {
        auto p = std::make_shared<value_type>(std::forward(args)...);
        p->self(p);
        return p;
    }

private:

    static constexpr auto TAG = "Model";

    std::weak_ptr<value_type> self_;

    Model(const self_type &) = delete;
    Model(self_type &&) = delete;

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

    std::weak_ptr<value_type> self() const {
        return self_;
    }

};

template<typename T>
class NestedModel : public Model<T> {

private:

    static constexpr auto TAG = "NestedModel";

    using value_type = typename T;
    using self_type = typename NestedModel<value_type>;

    std::weak_ptr<value_type> parent_;
    std::vector<std::shared_ptr<value_type>> children_;

    NestedModel(const self_type &) = delete;
    NestedModel(self_type &&) = delete;

    void parent(std::shared_ptr<value_type> p) {
        auto p_ = parent_.lock()
        if (p_ && p) {
            LOGE(TAG, "invalid parent");
            throw std::exception();
        }
        if (!p_ && !p) {
            return
        }
        parent_ = p;
        OnParentChanged();

    }

    virtual void OnParentChanged() {
    }

protected:

    std::shared_ptr<value_type> parent() {
        return parent_.lock();
    }

public:

    NestedModel() = default;
    ~NestedModel() = default;

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
                child->parent(nullptr);
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
