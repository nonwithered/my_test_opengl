#pragma once

#include "my_utils/log.h"

template<typename T>
class ResourceIdentify {

private:
    using value_type = T;
    using self_type = ResourceIdentify<T>;

public:
    using Resource = std::shared_ptr<value_type>;

private:

    static constexpr auto TAG = "ResourceIdentify";

    ResourceIdentify(self_type &&) = delete;

protected:

    virtual value_type *Create() const = 0;

public:

    ResourceIdentify() = default;

    ResourceIdentify(const self_type &) = default;

    virtual ~ResourceIdentify() = default;

    Resource Obtain() {
        return Resource(Create());
    }

    virtual size_t Hash() const = 0;

    virtual bool Equal(const self_type &) const = 0;
};

template <typename T>
struct std::hash<ResourceIdentify<T>> {
    size_t operator()(const ResourceIdentify<T> &r) const noexcept {
        return r.Hash();
    }
};

template <typename T>
struct std::equal_to<ResourceIdentify<T>> {
    bool operator()(const ResourceIdentify<T> &l, const ResourceIdentify<T> &r) const {
        return l.Equal(r);
    }
};
