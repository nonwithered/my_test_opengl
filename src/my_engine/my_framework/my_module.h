#pragma once

#include "my_framework/my_context.h"

class Module {

private:
    static constexpr auto TAG = "Module";

    Module(const Module &) = delete;
    Module(Module &&) = delete;

    std::vector<std::unique_ptr<Module>> pending_children_;
    std::vector<std::unique_ptr<Module>> children_;

    std::unordered_map<int, std::function<bool(Context &)>> key_event_press_;
    std::unordered_map<int, std::function<bool(Context &)>> key_event_release_;

    std::unordered_map<int, std::function<bool(Context &)>> mouse_button_event_press_;
    std::unordered_map<int, std::function<bool(Context &)>> mouse_button_event_release_;

    bool init_ = false;

    const char *type_name_ = "";

protected:

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

    virtual bool Frame(Context &context) {
        for (auto i = children_.begin(); i != children_.end(); ) {
            auto &module = *i;
            if (!module->PerformFrame(context)) {
                ++i;
            } else {
                LOGI(TAG, "close module");
                i = children_.erase(i);
            }
        }
        return false;
    }

    virtual void OnCreate(Global &context) {
    }

public:

    Module() = default;
    virtual ~Module() = default;

    const char *name() const {
        return type_name_;
    }

    bool PerformFrame(Context &context) {
        if (!init_) {
            init_ = true;
            LOGI(TAG, "OnCreate module %s", type_name_);
            OnCreate(context.global());
        }
        for (auto i = pending_children_.begin(); i != pending_children_.end(); ) {
            auto &module = *i;
            LOGI(TAG, "move module %s", module->type_name_);
            children_.push_back(std::move(module));
            i = pending_children_.erase(i);
        }
        return Frame(context);
    }

    void PerformKeyEvent(Context &context, int key, bool press) {
        if (KeyEvent(context, key, press)) {
            return;
        }
        for (auto &module : children_) {
            module->PerformKeyEvent(context, key, press);
        }
    }

    void PerformMouseButtonEvent(Context &context, int button, bool press) {
        if (MouseButtonEvent(context, button, press)) {
            return;
        }
        for (auto &module : children_) {
            module->PerformMouseButtonEvent(context, button, press);
        }
    }

    template<typename T, typename ...Args>
    void NewModule(Args... args) {
        auto type_name = typeid(T).name();
        LOGI(TAG, "NewModule %s", type_name);
        auto module = std::make_unique<T>(std::forward<Args>(args)...);
        module->type_name_ = type_name;
        pending_children_.push_back(std::move(module));
    }
};
