#pragma once

#include "my_framework/my_live_module.h"

class LauncherModule: public LiveModule, public LevelPresenter {

private:

    static constexpr auto TAG = "LauncherModule";

    bool init_ = false;

protected:

    bool alive(Global &context) final {
        return (bool) context.level().current();
    }

public:

    LauncherModule() : LiveModule() {
        LOGI(TAG, "ctor");
    }

    ~LauncherModule() {
        LOGI(TAG, "dtor");
    }

    void OnLevelStart(std::weak_ptr<Level>) override {
    }

};
