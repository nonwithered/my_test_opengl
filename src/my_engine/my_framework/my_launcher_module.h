#pragma once

#include "my_framework/my_live_module.h"

class LauncherModule: public LiveModule {

    friend class Runtime;

private:

    static constexpr auto TAG = "LauncherModule";

protected:

    bool alive(Global &context) final {
        return (bool) context.level().current();
    }

    virtual void OnCreate(Global &context) {
    }

    LauncherModule() : LiveModule() {
        LOGI(TAG, "ctor");
    }

public:

    ~LauncherModule() {
        LOGI(TAG, "dtor");
    }

};
