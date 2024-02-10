#pragma once

#include "my_framework/my_live_module.h"

class LauncherModule: public LiveModule {

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

};
