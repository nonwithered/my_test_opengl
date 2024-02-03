#pragma once

#include <log.h>

class Fraction {

private:

    static constexpr auto TAG = "Fraction";

    Fraction(const Fraction &) = delete;
    Fraction(Fraction &&) = delete;

    std::chrono::system_clock clock_ = std::chrono::system_clock();
    std::chrono::time_point<std::chrono::system_clock> last_time_ = clock_.now();

    std::chrono::seconds period_;

public:

    Fraction(unsigned int period) : period_(std::chrono::seconds(period)) {
    }


    float operator()() {
        auto this_time = clock_.now();
        auto delta_time = this_time - last_time_;
        last_time_ = this_time;
        return std::chrono::duration_cast<std::chrono::duration<float>>(delta_time) / period_;
    }
};
