#pragma once

#include "my_header/log.h"

class Interval {

private:

    static constexpr auto TAG = "Interval";

    Interval(const Interval &) = delete;
    Interval(Interval &&) = delete;

    std::chrono::system_clock clock_ = std::chrono::system_clock();
    std::chrono::time_point<std::chrono::system_clock> last_time_ = clock_.now();

    std::chrono::seconds period_;

public:

    Interval(unsigned int period) : period_(std::chrono::seconds(period)) {
    }


    float operator()() {
        auto this_time = clock_.now();
        auto delta_time = this_time - last_time_;
        last_time_ = this_time;
        return std::chrono::duration_cast<std::chrono::duration<float>>(delta_time) / period_;
    }
};
