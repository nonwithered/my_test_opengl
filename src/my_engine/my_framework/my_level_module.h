#pragma once

#include "my_framework/my_module.h"

class BasicModule : public Module{

    friend class LauncherModule;
    friend class LevelModule;

private:
    BasicModule(Context &context) : Module(context) {
    }

};

class LauncherModule: public BasicModule, public LevelPresenter  {

    friend class Window;

private:

    bool create_ = false;

    virtual void OnCreate() = 0;

protected:
    LauncherModule(Context &context) : BasicModule(context) {
    }

    void OnLevelStart(std::weak_ptr<Level>) override {
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

    std::weak_ptr<Level> level_;

    bool create_ = false;

    virtual void OnCreate() = 0;

protected:
    LevelModule(Context &context, std::weak_ptr<Level> level)
    : BasicModule(context)
    , level_(std::move(level)) {
    }

    bool Frame() final {
        if (!level_.lock()) {
            return false;
        }
        if (!create_) {
            create_ = true;
            OnCreate();
        }
        return BasicModule::Frame();
    }

};
