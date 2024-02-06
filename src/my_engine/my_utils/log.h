#pragma once

#include "my_utils/base.h"

#define _DO_ONCE(block) do block while (false)

inline const char *_log_time() {
    constexpr std::size_t kSize = 32;
    thread_local static std::array<char, kSize> str{};
    auto t = std::time(nullptr);
    auto *p = std::localtime(&t);
    std::strftime(str.data(), kSize, "%Y-%m-%d %H:%M:%S", p);
    return str.data();
}

inline int _log_thread_num() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    int id;
    ss >> id;
    return id;
}

#define _LOG(level, tag, msg, ...) fprintf(stderr, "%s %8d %s %s: " msg "\n", _log_time(), _log_thread_num(), level, tag, __VA_ARGS__)

#define _LOG_LEVEL_V 0
#define _LOG_LEVEL_D 1
#define _LOG_LEVEL_I 2
#define _LOG_LEVEL_W 3
#define _LOG_LEVEL_E 4
#define _LOG_LEVEL_A 5

#define _LOG_LEVEL _LOG_LEVEL_I

#define _LOG_IF(L, tag, msg, ...) _DO_ONCE({ if (_LOG_LEVEL_##L >= _LOG_LEVEL) _LOG(#L, tag, msg, ##__VA_ARGS__); })

#define LOGV(tag, msg, ...) _LOG_IF(V, tag, msg, ##__VA_ARGS__)

#define LOGD(tag, msg, ...) _LOG_IF(D, tag, msg, ##__VA_ARGS__)

#define LOGI(tag, msg, ...) _LOG_IF(I, tag, msg, ##__VA_ARGS__)

#define LOGW(tag, msg, ...) _LOG_IF(W, tag, msg, ##__VA_ARGS__)

#define LOGE(tag, msg, ...) _LOG_IF(E, tag, msg, ##__VA_ARGS__)
