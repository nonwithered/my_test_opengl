#pragma once

#include "my_utils/log.h"

class StringPool {

public:
    using Identify = typename uint64_t;

    static StringPool &Instance() {
        static StringPool instance_;
        return instance_;
    }

private:

    static constexpr auto TAG = "StringPool";

    using id_type = typename Identify;
    using value_type = typename std::string;


    std::mutex mutex_;

    std::unordered_map<id_type, value_type> pool_;
    std::unordered_map<value_type, id_type> inverse_pool_;

    id_type next_ = 0;

    std::lock_guard<std::mutex> Lock() {
        return std::lock_guard(mutex_);
    }

    StringPool() {
        LOGI(TAG, "ctor");
    }
    StringPool(const StringPool &) = delete;
    StringPool(StringPool &&) = delete;

public:
    ~StringPool() {
        LOGI(TAG, "dtor");
    }

    id_type Save(const value_type &name) {
        auto lock = Lock();
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
        auto lock = Lock();
        auto i = pool_.find(id);
        if (i == pool_.end()) {
            LOGE(TAG, "restore fail %" PRIu64, id);
            throw std::exception();
        }
        return i->second;
    }

};
