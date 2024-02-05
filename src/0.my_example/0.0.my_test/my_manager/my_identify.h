#pragma once

#include "my_utils/log.h"

template<typename T>
class ResourceIdentify {

private:
    using value_type = T;
    using self_type = typename ResourceIdentify<value_type>;

public:
    using Resource = typename std::shared_ptr<value_type>;

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
};
