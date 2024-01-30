#pragma once

#include <log.h>

void FPS() {
    static auto clock = std::chrono::system_clock();
    static auto last_time = clock.now();
    static auto count = 0;
    auto this_time = clock.now();
    ++count;
    auto delta_time = this_time - last_time;
    if (delta_time > std::chrono::seconds(1)) {
        LOGI("FPS", "%d", count);
        last_time = this_time;
        count = 0;
    }
}
