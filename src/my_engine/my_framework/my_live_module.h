#pragma once

#include "my_framework/my_module.h"

class LiveModule : public Module {

    static constexpr auto TAG = "ScopeModule";

protected:

    virtual bool alive(Global &context) = 0;

    bool Frame(Context &context) final {
        if (!alive(context.global())) {
            return true;
        }
        return OnFrame(context);
    }

    bool KeyEvent(Context &context, int key, bool press) final {
        if (!alive(context.global())) {
            return true;
        }
        return OnKeyEvent(context, key, press);
    }

    bool MouseButtonEvent(Context &context, int button, bool press) final {
        if (!alive(context.global())) {
            return true;
        }
        return OnMouseButtonEvent(context, button, press);
    }

    virtual bool OnFrame(Context &context) {
        return Module::Frame(context);
    }

    virtual bool OnKeyEvent(Context &context, int key, bool press) {
        return Module::KeyEvent(context, key, press);
    }

    virtual bool OnMouseButtonEvent(Context &context, int button, bool press) {
        return Module::MouseButtonEvent(context, button, press);
    }

public:

    LiveModule() = default;

    ~LiveModule() = default;
};
