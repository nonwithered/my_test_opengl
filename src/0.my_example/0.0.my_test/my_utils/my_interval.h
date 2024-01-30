#pragma once

#include <log.h>

class IntervalCounter {

private:
    std::chrono::system_clock clock_ = std::chrono::system_clock();
    std::chrono::time_point<std::chrono::system_clock> last_time_ = clock_.now();

public:
    float Count() {
        auto this_time = clock_.now();
        auto delta_time = this_time - last_time_;
        last_time_ = this_time;
        return std::chrono::duration_cast<std::chrono::duration<float>>(delta_time) / std::chrono::seconds(1);
    }
};
