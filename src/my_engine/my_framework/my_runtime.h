#pragma once

#include "my_utils/log.h"

#include "my_manager/my_level_manager.h"

#include "my_framework/my_window_manager.h"
#include "my_framework/my_scope_module.h"
#include "my_framework/my_launcher_module.h"

#include "my_utils/my_interval.h"

#include "my_model/my_model.h"

class Runtime : public Global, public LevelPresenter, public WindowPresenter {

public:

    template<typename T, typename ...Args>
    static Runtime Make(Args... args) {
        auto type_name = typeid(T).name();
        LOGI(TAG, "Runtime %s", type_name);
        return Runtime(std::make_unique<T>(std::forward<Args>(args)...));
    }

private:

    static constexpr auto TAG = "Runtime";

    Interval interval_ = Interval(1);

    float interval_fraction_ = 0.0f;

    LevelManager level_ = LevelManager(*this);
    WindowManager window_ = WindowManager(*this);

    std::unique_ptr<LauncherModule> module_;

    Runtime(const Runtime &) = delete;
    Runtime(Runtime &&) = default;

    Runtime(std::unique_ptr<LauncherModule> module) : module_(std::move(module)) {
        LOGI(TAG, "ctor");
        module_->OnCreate(*this);
    }

    void PerformFrame() {
        interval_fraction_ = interval_();
        LOGD(TAG, "PerformFrame %f", interval_fraction_);
        window_.PerformFrame(*module_);
        return ;
    }

public:

    ~Runtime() {
        if (!module_) {
            return;
        }
        LOGI(TAG, "dtor");
    }

    void Loop() {
        do {
            LOGD(TAG, "PerformFrame");
            PerformFrame();
            LOGD(TAG, "glfwPollEvents");
            glfwPollEvents();
        } while (level().current());
    }

    void OnLevelStart(std::weak_ptr<Level> level) final {
        module_->OnLevelStart(level);
    }

    float interval() override {
        return interval_fraction_;
    }

    LevelManager &level() override {
        return level_;
    }

    WindowManager &window() override {
        return window_;
    }

    operator Global &() override {
        return *this;
    }

    void FramebufferSizeCallback(Window &window, int width, int height) override {
        window.FramebufferSizeCallback(width, height);
        PerformFrame();
    }

    void KeyCallback(Window &window, int key, bool press) override {
        window.KeyCallback(*module_, key, press);
        PerformFrame();
    }

    void MouseButtonCallback(Window &window, int button, bool press) override {
        window.MouseButtonCallback(*module_, button, press);
        PerformFrame();
    }
};
