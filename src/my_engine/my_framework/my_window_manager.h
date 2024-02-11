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

    virtual operator Global &() = 0;

    virtual void FramebufferSizeCallback(Window &window, int width, int height) = 0;
    virtual void KeyCallback(Window &window, int key, bool press) = 0;
    virtual void MouseButtonCallback(Window &window, int button, bool press) = 0;

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
        glfwSetFramebufferSizeCallback(w.id(), [](GLFWwindow *window, int width, int height) {
            auto &manager = Instance();
            auto &context = manager.Find(window);
            LOGD(TAG, "FramebufferSizeCallback %s %d %d", context.title().data(), width, height);
            manager.presenter_.FramebufferSizeCallback(context, width, height);
        });
        glfwSetKeyCallback(w.id(), [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_UNKNOWN) {
                return;
            }
            if (action != GLFW_PRESS && action != GLFW_RELEASE) {
                return;
            }
            auto press = action == GLFW_PRESS;
            auto &manager = Instance();
            auto &context = manager.Find(window);
            LOGD(TAG, "KeyCallback %s %d %d %d %d", context.title().data(), key, scancode, action, mods);
            manager.presenter_.KeyCallback(context, key, press);
        });
        glfwSetMouseButtonCallback(w.id(), [](GLFWwindow *window, int button, int action, int mods) {
            if (action != GLFW_PRESS && action != GLFW_RELEASE) {
                return;
            }
            auto press = action == GLFW_PRESS;
            auto &manager = Instance();
            auto &context = manager.Find(window);
            LOGD(TAG, "MouseButtonCallback %s %d %d %d", context.title().data(), button, action, mods);
            manager.presenter_.MouseButtonCallback(context, button, press);
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
            if (window->id() == id) {
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
        for (auto &window : windows_) {
            window->PerformFrame(module);
        }
    }

    void NewWindow(const std::string &title, int width, int height) {
        LOGI(TAG, "NewWindow %s", title.data());
        auto window = std::make_unique<Window>(presenter_, title, width, height, [this](Window &w) {
            SetupWindow(w);
        });
        pending_windows_.push_back(std::move(window));
    }

};
