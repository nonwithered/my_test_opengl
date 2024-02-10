#pragma once

#include "my_framework/my_context.h"

class Module {

    friend class Window;

private:
    static constexpr auto TAG = "Module";

    Module(const Module &) = delete;
    Module(Module &&) = delete;

    Context *context_ = nullptr;

    std::vector<std::unique_ptr<Module>> pending_children_;
    std::vector<std::unique_ptr<Module>> children_;

    std::unordered_map<int, std::function<bool()>> key_event_press_;
    std::unordered_map<int, std::function<bool()>> key_event_release_;

    std::unordered_map<int, std::function<bool()>> mouse_button_event_press_;
    std::unordered_map<int, std::function<bool()>> mouse_button_event_release_;

    bool init_ = false;

    bool PerformFrame() {
        for (auto i = pending_children_.begin(); i != pending_children_.end(); ) {
            auto &module = *i;
            children_.push_back(std::move(module));
            i = pending_children_.erase(i);
        }
        if (!init_) {
            init_ = true;
            Create();
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

protected:
    Module() = default;

    Context &context() {
        return *context_;
    }

    void ListenKeyEvent(int key, bool press, std::function<bool()> event) {
        auto &events = press ? key_event_press_ : key_event_release_;
        if (event) {
            events[key] = std::move(event);
        } else {
            events.erase(key);
        }
    }

    void ListenMouseButtonEvent(int button, bool press, std::function<bool()> event) {
        auto &events = press ? mouse_button_event_press_ : mouse_button_event_release_;
        if (event) {
            events[button] = std::move(event);
        } else {
            events.erase(button);
        }
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

    virtual void Create() {
    }

public:

    virtual ~Module() = default;

    template<typename T, typename ...Args>
    void NewModule(Args... args) {
        LOGI(TAG, "NewModule %s", typeid(T).name());
        auto module = std::make_unique<T>(std::forward<Args>(args)...);
        module->context_ = context_;
        pending_children_.push_back(std::move(module));
    }
};
