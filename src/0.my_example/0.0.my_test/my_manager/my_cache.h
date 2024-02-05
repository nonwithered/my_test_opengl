#pragma once

#include "my_utils/log.h"

#include "my_manager/my_identify.h"

template<typename T>
class CacheManager {

private:

    using Identify = T;
    using self_type = typename CacheManager<Identify>;
    using key_type = typename std::shared_ptr<Identify>;
    using mapped_type = typename Identify::Resource;

    static constexpr auto TAG = "CacheManager";

    CacheManager(const self_type &) = delete;
    CacheManager(self_type &&) = delete;

    std::unordered_map<key_type, mapped_type> cache_;

public:

    CacheManager() = default;
    ~CacheManager() = default;

    void erase(const key_type &identify) {
        cache_.erase(identify);
    }

    mapped_type find(const key_type &identify) {
        auto i = cache_.find(identify);
        if (i == cache_.end()) {
            mapped_type r = identify.Obtain()
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
