#pragma once

#include "my_utils/log.h"

#include "my_framework/my_window.h"

#include "my_framework/my_launcher_module.h"
#include "my_framework/my_scope_module.h"

#include "my_utils/my_counter.h"
#include "my_utils/my_interval.h"

#include "my_model/my_model.h"

class Runtime : public Global, public LevelPresenter {

public:

    template<typename T, typename ...Args>
    static Runtime Make(Args... args) {
        auto type_name = typeid(T).name();
        LOGI(TAG, "Runtime %s", type_name);
        auto module = std::make_unique<T>(std::forward<Args>(args)...);
        return Runtime(std::move(module));
    }

private:

    static constexpr auto TAG = "Runtime";

    bool init_gl_ = false;

    std::vector<std::unique_ptr<Window>> windows_;
    std::vector<std::unique_ptr<Window>> pending_windows_;

    Counter counter_ = Counter(1);
    Interval interval_ = Interval(1);

    float interval_fraction_ = 0.0f;

    LevelManager level_ = LevelManager(*this);

    std::unique_ptr<LauncherModule> module_;

    Runtime(const Runtime &) = delete;
    Runtime(Runtime &&) = default;

    static Runtime &Instance() {
        return *Instance(nullptr);
    }

    static Runtime *Instance(Runtime *p) {
        static Runtime *instance_ = nullptr;
        if (p != nullptr && instance_ == nullptr) {
            instance_ = p;
            return nullptr;
        }
        if (p == nullptr && instance_ != nullptr) {
            return instance_;
        }
        if (p == instance_) {
            instance_ = nullptr;
            return nullptr;
        }
        LOGE(TAG, "Instance invalid %p %p", p, instance_);
        throw std::exception();
    }

    Window &Find(GLFWwindow *id) {
        for (auto &window : windows_) {
            if (window->IsSame(id)) {
                return *window;
            }
        }
        LOGE(TAG, "Find fail %p", id);
        throw std::exception();
    }

    bool PerformFrame() {
        interval_fraction_ = interval_();
        LOGD(TAG, "PerformFrame %f", interval_fraction_);
        {
            auto count = counter_();
            if (count > 0) {
                LOGI(TAG, "FPS %u", count);
            }
        }
        for (auto i = pending_windows_.begin(); i != pending_windows_.end(); ) {
            auto &window = *i;
            LOGI(TAG, "move window %s", window->title().data());
            windows_.push_back(std::move(window));
            i = pending_windows_.erase(i);
        }
        for (auto i = windows_.begin(); i != windows_.end(); ++i) {
            auto &window = *i;
            window->PerformFrame(*module_);
        }
        return !level().current();
    }

    void SetupWindow(Window &w) {
        if (!init_gl_) {
            init_gl_ = true;
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                LOGE(TAG, "gladLoadGLLoader fail");
                throw std::exception();
            }
        }
        w.SetFramebufferSizeCallback([](GLFWwindow *window, int width, int height) {
            auto &runtime = Instance();
            runtime.Find(window).FramebufferSizeCallback(width, height);
            runtime.PerformFrame();
        });
        w.SetKeyCallback([](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto &runtime = Instance();
            runtime.Find(window).KeyCallback(*runtime.module_, key, scancode, action, mods);
            runtime.PerformFrame();
        });
        w.SetMouseButtonCallback([](GLFWwindow *window, int button, int action, int mods) {
            auto &runtime = Instance();
            runtime.Find(window).MouseButtonCallback(*runtime.module_, button, action, mods);
            runtime.PerformFrame();
        });
    }

    Runtime(std::unique_ptr<LauncherModule> module) : module_(std::move(module)) {

        Instance(this);

        LOGI(TAG, "ctor");

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
    }

public:

    ~Runtime() {
        if (!module_) {
            return;
        }
        // TODO
        windows_.clear();
        pending_windows_.clear();

        glfwTerminate();
        Instance(this);
        LOGI(TAG, "dtor");
    }

    void NewWindow(const std::string &title, int width, int height) {
        LOGI(TAG, "NewWindow %s", title.data());
        auto window = std::make_unique<Window>(*this, title, width, height, [this](Window &w) {
            SetupWindow(w);
        });
        pending_windows_.push_back(std::move(window));
    }

    void Loop() {
        while (!PerformFrame()) {
            LOGD(TAG, "glfwPollEvents");
            glfwPollEvents();
        }
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
};
