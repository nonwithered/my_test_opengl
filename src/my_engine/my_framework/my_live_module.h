#pragma once

#include "my_framework/my_module.h"

class LiveModule : public Module {

    static constexpr auto TAG = "ScopeModule";

private:

    virtual bool alive() = 0;

protected:

    void Create() final {
        if (!alive()) {
            LOGW(TAG, "Create but not alive");
            return;
        }
        OnCreate();
    }

    bool Frame() final {
        if (!alive()) {
            return true;
        }
        return OnFrame();
    }

    bool KeyEvent(int key, bool press) final {
        if (!alive()) {
            return true;
        }
        return OnKeyEvent(key, press);
    }

    bool MouseButtonEvent(int button, bool press) final {
        if (!alive()) {
            return true;
        }
        return OnMouseButtonEvent(button, press);
    }

    virtual void OnCreate() {
        Module::Create();
    }

    virtual bool OnFrame() {
        return Module::Frame();
    }

    virtual bool OnKeyEvent(int key, bool press) {
        return Module::KeyEvent(key, press);
    }

    virtual bool OnMouseButtonEvent(int button, bool press) {
        return Module::MouseButtonEvent(button, press);
    }

public:

    LiveModule() = default;

    ~LiveModule() = default;
};
