#pragma once

#include "my_utils/log.h"

#include "my_manager/my_identify.h"

template<typename Identify>
class CacheManager {

private:

    using Resource = Identify::Resource;

    static constexpr auto TAG = "CacheManager";

    CacheManager(const CacheManager<T> &) = delete;
    CacheManager(CacheManager<T> &&) = delete;

    std::unordered_map<Identify, Resource> cache_;

public:

    CacheManager() = default;
    ~CacheManager() = default;

    void erase(const Identify &identify) {
        cache_.erase(identify);
    }

    Resource find(const Identify &identify) {
        auto i = cache_.find(identify);
        if (i == cache_.end()) {
            Resource r = identify.Obtain()
            cache_.insert(identify, r);
            return r;
        }
        auto [k, v] = *i;
        if (!v) {
            LOGW(TAG, "find contains but false");
            v = k.Obtain();
            *i = v;
            return v;
        }
        return v;
    }

};
