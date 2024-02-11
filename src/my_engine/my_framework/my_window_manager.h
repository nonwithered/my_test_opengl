#pragma once

#include "my_utils/log.h"

#include "my_framework/my_module.h"
#include "my_framework/my_window.h"

#include "my_utils/my_counter.h"

class WindowPresenter {

private:
    WindowPresenter(const WindowPresenter &) = delete;
    WindowPresenter(WindowPresenter &&) = delete;

protected:
    WindowPresenter() = default;

public:
    virtual ~WindowPresenter() = default;

    virtual void NotifyFlush() = 0;
    virtual Module &module() = 0;
    virtual Global &global() = 0;

};

class WindowManager {

private:

    static WindowManager &Instance() {
        return *Instance(nullptr);
    }

    static WindowManager *Instance(WindowManager *p) {
        static WindowManager *instance_ = nullptr;
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

    static constexpr auto TAG = "WindowManager";

    WindowManager(const WindowManager &) = delete;
    WindowManager(WindowManager &&) = delete;

    WindowPresenter &presenter_;

    bool init_ = false;
    Counter counter_ = Counter(1);

    std::vector<std::unique_ptr<Window>> windows_;
    std::vector<std::unique_ptr<Window>> pending_windows_;

    void SetupWindow(Window &w) {
        if (!init_) {
            init_ = true;
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                LOGE(TAG, "gladLoadGLLoader fail");
                throw std::exception();
            }
        }
        w.SetFramebufferSizeCallback([](GLFWwindow *window, int width, int height) {
            auto &manager = Instance();
            manager.Find(window).FramebufferSizeCallback(width, height);
            manager.presenter_.NotifyFlush();
        });
        w.SetKeyCallback([](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto &manager = Instance();
            manager.Find(window).KeyCallback(manager.presenter_.module(), key, scancode, action, mods);
            manager.presenter_.NotifyFlush();
        });
        w.SetMouseButtonCallback([](GLFWwindow *window, int button, int action, int mods) {
            auto &manager = Instance();
            manager.Find(window).MouseButtonCallback(manager.presenter_.module(), button, action, mods);
            manager.presenter_.NotifyFlush();
        });
    }

public:

    WindowManager(WindowPresenter &presenter) : presenter_(presenter) {
        LOGI(TAG, "ctor");
        Instance(this);

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
    }

    ~WindowManager() {
        LOGI(TAG, "dtor");
        Instance(this);

        windows_.clear();
        pending_windows_.clear();
        glfwTerminate();
        init_ = false;
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

    void PerformFrame(Module &module) {
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
            window->PerformFrame(module);
        }
    }

    void NewWindow(const std::string &title, int width, int height) {
        LOGI(TAG, "NewWindow %s", title.data());
        auto window = std::make_unique<Window>(presenter_.global(), title, width, height, [this](Window &w) {
            SetupWindow(w);
        });
        pending_windows_.push_back(std::move(window));
    }

};
