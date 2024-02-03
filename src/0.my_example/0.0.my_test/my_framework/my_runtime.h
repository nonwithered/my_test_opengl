#pragma once

#include <log.h>

#include "my_window.h"

#include "my_utils/my_counter.h"

class Runtime {

private:

    static constexpr auto TAG = "Runtime";

    bool init_gl_ = false;

    std::list<Window> windows_;
    std::list<Window> pending_windows_;

    Counter counter_ = Counter(1);

    Runtime(const Runtime &) = delete;
    Runtime(Runtime &&) = delete;

    static Runtime *Instance(Runtime *p = nullptr) {
        static Runtime *instance_ = nullptr;
        if (p != nullptr && instance_ == nullptr) {
            instance_ = p;
            return nullptr;
        }
        if (p == nullptr && instance_ != nullptr) {
            return instance_;
        }
        LOGE(TAG, "Instance invalid %p %p", p, instance_);
        throw std::exception();
    }

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
        Instance()->Find(window).FramebufferSizeCallback(width, height);
        Instance()->PerformFrame();
    }
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        Instance()->Find(window).KeyCallback(key, scancode, action, mods);
        Instance()->PerformFrame();
    }

    void SetCallback() {
        Window::Callback(glfwSetFramebufferSizeCallback, FramebufferSizeCallback);
        Window::Callback(glfwSetKeyCallback, KeyCallback);
    }

    Window &Find(GLFWwindow *id) {
        for (auto &window : windows_) {
            if (window.IsSame(id)) {
                return window;
            }
        }
        LOGE(TAG, "Find fail %p", id);
        throw std::exception();
    }

    void PerformFrame() {
        {
            auto count = counter_();
            if (count > 0) {
                LOGI(TAG, "FPS %u", count);
            }
        }
        for (auto i = pending_windows_.begin(); i != pending_windows_.end(); ) {
            auto &window = *i;
            LOGI(TAG, "move window %s", window.title().data());
            windows_.push_back(std::move(window));
            i = pending_windows_.erase(i);
        }
        for (auto i = windows_.begin(); i != windows_.end();) {
            auto &window = *i;
            if (window.IsClosed()) {
                LOGI(TAG, "close window %s", window.title().data());
                i = windows_.erase(i);
            } else {
                ++i;
                window.PerformFrame();
            }
        }
    }

public:

    Runtime() {

        Instance(this);

        LOGI(TAG, "ctor");

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        SetCallback();
    }

    ~Runtime() {
        glfwTerminate();
        LOGI(TAG, "dtor");
    }

    void NewWindow(const std::string &title, int width, int height, std::function<void(Window &)> init) {
        LOGI(TAG, "add window %s", title.data());
        auto window = Window(title, width, height, [this, &init](Window &w) {
            if (!init_gl_) {
                init_gl_ = true;
                if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                    LOGE(TAG, "gladLoadGLLoader fail");
                    throw std::exception();
                }
            }
            if (init) {
                init(w);
            } else {
                LOGW(TAG, "NewWindow invalid init");
            }
        });
        pending_windows_.push_back(std::move(window));
    }

    void Loop() {
        while (!windows_.empty() || !pending_windows_.empty()) {
            PerformFrame();
            glfwPollEvents();
        }
    }
};
