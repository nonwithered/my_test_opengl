#pragma once

#include "my_framework/my_window_monitor.h"

class Module : public WindowMonitor {

private:
    static constexpr auto TAG = "Module";

    Module(const Module &) = delete;
    Module(Module &&) = delete;

    const char *type_name_ = "";

    std::vector<std::unique_ptr<Module>> pending_children_;
    std::vector<std::unique_ptr<Module>> children_;

protected:

    bool Frame(Context &context) override {
        for (auto i = children_.begin(); i != children_.end(); ) {
            auto &module = *i;
            if (!module->PerformFrame(context)) {
                ++i;
            } else {
                LOGI(TAG, "close module");
                i = children_.erase(i);
            }
        }
        return WindowMonitor::Frame(context);
    }

public:

    Module() = default;
    virtual ~Module() = default;

    const char *name() const {
        return type_name_;
    }

    bool PerformFrame(Context &context) {
        for (auto i = pending_children_.begin(); i != pending_children_.end(); ) {
            auto &module = *i;
            LOGI(TAG, "move module %s", module->type_name_);
            children_.push_back(std::move(module));
            i = pending_children_.erase(i);
        }
        return Frame(context);
    }

    void PerformFramebufferSize(Context &context, int width, int height) {
        OnFramebufferSize(context, width, height);
        for (auto &module : children_) {
            module->PerformFramebufferSize(context, width, height);
        }
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
