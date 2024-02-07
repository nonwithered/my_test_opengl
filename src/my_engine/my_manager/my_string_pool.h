#pragma once

#include "my_utils/log.h"

class StringPool {

public:
    using id_type = typename uint64_t;
    using value_type = typename std::string;

private:

    static constexpr auto TAG = "StringPool";

    StringPool(const StringPool &) = default;
    StringPool(StringPool &&) = delete;

    std::mutex mutex_;

    std::unordered_map<id_type, value_type> pool_;
    std::unordered_map<value_type, id_type> inverse_pool_;

    id_type next_ = 0;

    std::lock_guard<std::mutex> Guard() {
        return std::lock_guard(mutex_);
    }

public:

    StringPool() = default;
    ~StringPool() = default;

    id_type Save(const value_type &name) {
        auto guard_ = Guard();
        auto [i, b] = inverse_pool_.emplace(name, next_);
        if (b) {
            ++next_;
            b = pool_.emplace(i->second, i->first).second;
            if (!b) {
                LOGE(TAG, "save fail %s", name.data());
                throw std::exception();
            }
        }
        return i->second;
    }

    value_type Restore(const id_type &id) {
        auto guard_ = Guard();
        auto i = pool_.find(id);
        if (i == pool_.end()) {
            LOGE(TAG, "restore fail %" PRIu64, (uint64_t) id);
            throw std::exception();
        }
        return i->second;
    }

};
