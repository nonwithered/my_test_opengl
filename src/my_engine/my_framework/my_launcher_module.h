#pragma once

#include "my_framework/my_live_module.h"

class LauncherModule: public LiveModule, public LevelPresenter {

private:

    static constexpr auto TAG = "LauncherModule";

    bool init_ = false;

    bool alive() final {
        return !init_ || !context().global().level().empty();
    }

protected:
    LauncherModule() : LiveModule() {
        LOGI(TAG, "ctor");
    }

    ~LauncherModule() {
        LOGI(TAG, "dtor");
    }

    void OnCreate() override {
        init_ = true;
        return LiveModule::OnCreate();
    }

public:

    void OnLevelStart(std::weak_ptr<Level>) override {
    }

};
