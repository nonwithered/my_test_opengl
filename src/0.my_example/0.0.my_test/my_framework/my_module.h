#pragma once

#include "my_framework/my_context.h"

class Module {

private:
    static constexpr auto TAG = "Module";
    bool finish_ = false;

    Module(const Module &) = delete;
    Module(Module &&) = delete;

    Context &context_;

    std::vector<std::unique_ptr<Module>> pending_children_;
    std::vector<std::unique_ptr<Module>> children_;

    std::unordered_map<int, std::function<bool()>> key_event_press_;
    std::unordered_map<int, std::function<bool()>> key_event_release_;

protected:

    Context &context() {
        return context_;
    }

    virtual void Frame(float fraction) {
        for (auto i = children_.begin(); i != children_.end(); ) {
            auto &module = *i;
            if (module->IsClosed()) {
                LOGI(TAG, "close module");
                i = children_.erase(i);
            } else {
                ++i;
                module->PerformFrame(fraction);
            }
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

    void Close() {
        finish_ = true;
    }

    void KeyEvent(int key, bool press, std::function<bool()> event) {
        auto &events = press ? key_event_press_ : key_event_release_;
        if (event) {
            events[key] = std::move(event);
        } else {
            events.erase(key);
        }
    }

public:
    Module(Context &context): context_(context) {
    }

    virtual ~Module() = default;

    bool IsClosed() {
        return finish_;
    }

    void NewModule(std::unique_ptr<Module> module) {
        LOGI(TAG, "NewModule");
        pending_children_.push_back(std::move(module));
    }

    void PerformKeyEvent(int key, bool press) {
        if (KeyEvent(key, press)) {
            return;
        }
        for (auto i = children_.begin(); i != children_.end(); ++i) {
            auto &module = *i;
            module->PerformKeyEvent(key, press);
        }
    }

    void PerformFrame(float fraction) {
        for (auto i = pending_children_.begin(); i != pending_children_.end(); ) {
            auto &module = *i;
            children_.push_back(std::move(module));
            i = pending_children_.erase(i);
        }
        Frame(fraction);
    }
};
