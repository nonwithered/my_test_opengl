#pragma once

#include "my_framework/my_module.h"

class BasicModule : public Module {

    friend class LauncherModule;
    friend class LevelModule;

    static constexpr auto TAG = "BasicModule";

private:
    BasicModule(Context &context) : Module(context) {
    }

};

class LauncherModule: public BasicModule {

    friend class Window;

    static constexpr auto TAG = "LauncherModule";

private:

    bool create_ = false;

    virtual void OnCreate() {
    }

protected:
    LauncherModule(Context &context) : BasicModule(context) {
    }

    bool Frame() final {
        if (!create_) {
            create_ = true;
            OnCreate();
        } else if (context().global().level().empty()) {
            return true;
        }
        return BasicModule::Frame();
    }

};

class LevelModule: public BasicModule {

private:

    static constexpr auto TAG = "LevelModule";

    std::weak_ptr<Level> level_;

    bool create_ = false;

    virtual void OnCreate() {
    }

protected:
    LevelModule(Context &context, std::weak_ptr<Level> level)
    : BasicModule(context)
    , level_(std::move(level)) {
    }

    std::shared_ptr<Level> level() {
        return level_.lock();
    }

    bool Frame() final {
        if (!level()) {
            return true;
        }
        if (!create_) {
            create_ = true;
            OnCreate();
        }
        return BasicModule::Frame();
    }
    bool KeyEvent(int key, bool press) final {
        if (!level()) {
            return true;
        }
        return BasicModule::KeyEvent(key, press);
    }
    bool MouseButtonEvent(int button, bool press) final {
        if (!level()) {
            return true;
        }
        return BasicModule::MouseButtonEvent(button, press);
    }
};
