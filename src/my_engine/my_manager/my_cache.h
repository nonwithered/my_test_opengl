#pragma once

#include "my_utils/my_log.h"

#include "my_manager/my_identify.h"

template<typename T>
class CacheManager {

private:

    using key_type = typename T;
    using key_ptr = typename std::unique_ptr<key_type>;
    using mapped_type = typename key_type::value_type;
    using mapped_ptr = typename std::shared_ptr<mapped_type>;
    using self_type = typename CacheManager<key_type>;

    static constexpr auto TAG = "CacheManager";

    CacheManager(const self_type &) = delete;
    CacheManager(self_type &&) = delete;

    // TODO
    std::list<std::pair<key_ptr, mapped_ptr>> cache_;

public:

    CacheManager() = default;
    ~CacheManager() = default;

    void erase(const key_type &identify) {
        for (auto i = cache_.begin(); i != cache_.end(); ++i) {
            auto &p = i->first; 
            if (identify.Hash() == p->Hash() && identify.Equal(*p)) {
                cache_.erase(i);
            }
        }
    }

    mapped_ptr find(const key_type &identify) {
        for (auto i = cache_.begin(); i != cache_.end(); ++i) {
            auto &p = i->first; 
            if (identify.Hash() == p->Hash() && identify.Equal(*p)) {
                return i->second;
            }
        }
        cache_.emplace_front(identify.Clone(), mapped_ptr(identify.Obtain().release()));
        return cache_.front().second;
    }

};
