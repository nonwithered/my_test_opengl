#pragma once

#include "my_framework/my_context.h"

class WindowMonitor : public FrameMonitor {

private:
    static constexpr auto TAG = "WindowMonitor";

    WindowMonitor(const WindowMonitor &) = delete;
    WindowMonitor(WindowMonitor &&) = delete;

    std::unordered_map<int, std::function<bool(Context &)>> key_event_press_;
    std::unordered_map<int, std::function<bool(Context &)>> key_event_release_;

    std::unordered_map<int, std::function<bool(Context &)>> mouse_button_event_press_;
    std::unordered_map<int, std::function<bool(Context &)>> mouse_button_event_release_;

public:
    virtual ~WindowMonitor() = default;

protected:

    WindowMonitor() = default;

    void ListenKeyEvent(int key, bool press, std::function<bool(Context &)> event) {
        auto &events = press ? key_event_press_ : key_event_release_;
        if (event) {
            events[key] = std::move(event);
        } else {
            events.erase(key);
        }
    }

    void ListenMouseButtonEvent(int button, bool press, std::function<bool(Context &)> event) {
        auto &events = press ? mouse_button_event_press_ : mouse_button_event_release_;
        if (event) {
            events[button] = std::move(event);
        } else {
            events.erase(button);
        }
    }


    virtual bool KeyEvent(Context &context, int key, bool press) {
        auto &events = press ? key_event_press_ : key_event_release_;
        auto i = events.find(key);
        if (i != events.end()) {
            auto &event = i->second;
            if (event && event(context)) {
                return true;
            }
        }
        return false;
    }
    virtual bool MouseButtonEvent(Context &context, int button, bool press) {
        auto &events = press ? mouse_button_event_press_ : mouse_button_event_release_;
        auto i = events.find(button);
        if (i != events.end()) {
            auto &event = i->second;
            if (event && event(context)) {
                return true;
            }
        }
        return false;
    }

};
