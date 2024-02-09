#pragma once

#include "my_framework/my_context.h"

class Module {

private:
    static constexpr auto TAG = "Module";

    Module(const Module &) = delete;
    Module(Module &&) = delete;

    Context &context_;

    std::vector<std::unique_ptr<Module>> pending_children_;
    std::vector<std::unique_ptr<Module>> children_;

    std::unordered_map<int, std::function<bool()>> key_event_press_;
    std::unordered_map<int, std::function<bool()>> key_event_release_;

    std::unordered_map<int, std::function<bool()>> mouse_button_event_press_;
    std::unordered_map<int, std::function<bool()>> mouse_button_event_release_;

protected:
    Module(Context &context): context_(context) {
    }

    Context &context() {
        return context_;
    }

    virtual bool Frame() {
        for (auto i = children_.begin(); i != children_.end(); ) {
            auto &module = *i;
            if (!module->PerformFrame()) {
                ++i;
            } else {
                LOGI(TAG, "close module");
                i = children_.erase(i);
            }
        }
        return false;
    }

    virtual bool KeyEvent(int key, bool press) {
        auto &events = press ? key_event_press_ : key_event_release_;
        auto i = events.find(key);
        if (i != events.end()) {
            auto &event = i->second;
            if (event && event()) {
                return true;
            }
        }
        return false;
    }

    void KeyEvent(int key, bool press, std::function<bool()> event) {
        auto &events = press ? key_event_press_ : key_event_release_;
        if (event) {
            events[key] = std::move(event);
        } else {
            events.erase(key);
        }
    }

    virtual bool MouseButtonEvent(int button, bool press) {
        auto &events = press ? mouse_button_event_press_ : mouse_button_event_release_;
        auto i = events.find(button);
        if (i != events.end()) {
            auto &event = i->second;
            if (event && event()) {
                return true;
            }
        }
        return false;
    }

    void MouseButtonEvent(int button, bool press, std::function<bool()> event) {
        auto &events = press ? mouse_button_event_press_ : mouse_button_event_release_;
        if (event) {
            events[button] = std::move(event);
        } else {
            events.erase(button);
        }
    }

public:

    virtual ~Module() = default;

    void NewModule(std::unique_ptr<Module> module) {
        LOGI(TAG, "NewModule");
        pending_children_.push_back(std::move(module));
    }

    bool PerformFrame() {
        for (auto i = pending_children_.begin(); i != pending_children_.end(); ) {
            auto &module = *i;
            children_.push_back(std::move(module));
            i = pending_children_.erase(i);
        }
        return Frame();
    }

    void PerformKeyEvent(int type, bool press) {
        if (KeyEvent(type, press)) {
            return;
        }
        for (auto &module : children_) {
            module->PerformKeyEvent(type, press);
        }
    }

    void PerformMouseButtonEvent(int type, bool press) {
        if (MouseButtonEvent(type, press)) {
            return;
        }
        for (auto &module : children_) {
            module->PerformMouseButtonEvent(type, press);
        }
    }
};
