#pragma once

#include "my_header/log.h"

class Counter {

private:

    static constexpr auto TAG = "Counter";

    Counter(const Counter &) = delete;
    Counter(Counter &&) = delete;

    std::chrono::system_clock clock_;
    std::chrono::time_point<decltype(clock_)> last_time_;
    unsigned int count_ = 0;

    std::chrono::seconds period_;

public:

    Counter(unsigned int period) : period_(std::chrono::seconds(period)) {
    }

    unsigned int operator()() {
        ++count_;
        auto now_time = clock_.now();
        auto delta_time = now_time - last_time_;
        if (delta_time < period_) {
            return 0;
        }
        auto count = count_;
        count_ = 0;
        last_time_ = now_time;
        return count;
    }

};
