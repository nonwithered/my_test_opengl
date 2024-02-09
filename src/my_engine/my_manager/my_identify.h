#pragma once

#include "my_utils/log.h"

template<typename T>
class ResourceIdentify {

public:
    using value_type = typename T;

public:
    using self_type = typename ResourceIdentify<value_type>;

private:

    static constexpr auto TAG = "ResourceIdentify";

    ResourceIdentify(self_type &&) = delete;

    ResourceIdentify(const self_type &) = delete;

protected:

    ResourceIdentify() = default;

public:

    virtual ~ResourceIdentify() = default;

    virtual std::unique_ptr<value_type> Obtain() const = 0;

    virtual std::unique_ptr<self_type> Clone() const = 0;

    virtual uint64_t Hash() const = 0;

    virtual bool Equal(const self_type &) const = 0;
};
