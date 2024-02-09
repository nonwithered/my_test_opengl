#pragma once

#include "my_utils/log.h"

class Defer {

private:

    static constexpr auto TAG = "Defer";

    Defer(const Defer &) = delete;

    std::function<void()> defer_;

public:

    Defer(std::function<void()> defer): defer_(std::move(defer)) {
    }

    Defer(Defer &&that) : defer_(std::move(that.defer_)) {
    }

    ~Defer() {
        if (!defer_) {
            return;
        }
        defer_();
        defer_ = nullptr;
    }

    operator bool() const {
        return defer_.operator bool();
    }

};
