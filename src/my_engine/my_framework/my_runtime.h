#pragma once

#include "my_utils/my_log.h"

#include "my_manager/my_level_manager.h"

#include "my_framework/my_window_manager.h"
#include "my_framework/my_scope_module.h"
#include "my_framework/my_launcher_module.h"

#include "my_utils/my_interval.h"
#include "my_utils/my_counter.h"

#include "my_model/my_model.h"

class Runtime : public Global, public WindowPresenter {

public:

    template<typename T, typename ...Args>
    static Runtime Make(Args... args) {
        auto type_name = typeid(T).name();
        LOGI(TAG, "Runtime %s", type_name);
        return Runtime(std::make_unique<T>(std::forward<Args>(args)...));
    }

private:

    static constexpr auto TAG = "Runtime";

    Counter counter_ = Counter(1);
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
        {
            auto count = counter_();
            if (count > 0) {
                LOGI(TAG, "FPS %u", count);
            }
        }
        interval_fraction_ = interval_();
        LOGD(TAG, "PerformFrame %f", interval_fraction_);
        window_.PerformFrame(*module_);
        level_.PerformFrame([this](Module &module) {
            window_.PerformFrame(module);
        });
        window_.SwapBuffers();
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

    void OnFramebufferSize(Window &window, int width, int height) override {
        window.OnFramebufferSize(width, height);
        PerformFrame();
    }

    void PerformKeyEvent(Window &window, int key, bool press) override {
        level_.PerformKeyEvent(window, key, press);
        window.PerformKeyEvent(*module_, key, press);
        PerformFrame();
    }

    void PerformMouseButtonEvent(Window &window, int button, bool press) override {
        level_.PerformMouseButtonEvent(window, button, press);
        window.PerformMouseButtonEvent(*module_, button, press);
        PerformFrame();
    }

    void OnWindowClose(Window &window) override {
        level_.OnWindowClose(window);
        window_.OnWindowClose(window);
        PerformFrame();
    }
};
