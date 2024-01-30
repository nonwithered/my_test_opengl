#include <log.h>

const char *_log_time() {
    constexpr std::size_t kSize = 32;
    thread_local static std::array<char, kSize> str{};
    auto t = std::time(nullptr);
    auto *p = std::localtime(&t);
    std::strftime(str.data(), kSize, "%Y-%m-%d %H:%M:%S", p);
    return str.data();
}

int _log_thread_num() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    int id;
    ss >> id;
    return id;
}
