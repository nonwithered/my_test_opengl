#pragma once

#include <log.h>

#include "my_window.h"

#include "my_utils/my_fps.h"

class Runtime {

private:
    Runtime(const Runtime &) = delete;
    Runtime(Runtime &&) = delete;

private:

    static constexpr auto TAG = "Runtime";

    static Runtime *runtime;

    std::list<Window> windows_;
    std::list<Window> pending_windows_;

    Window &FindWindow(GLFWwindow *id) {
        for (auto &window : windows_) {
            if (window.IsSame(id)) {
                return window;
            }
        }
        LOGE(TAG, "FindWindow fail %p", id);
        throw std::exception();
    }

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
        runtime->FindWindow(window).OnSizeChanged(width, height);
    }

    void MoveWindows() {
        for (auto i = pending_windows_.begin(); i != pending_windows_.end(); ) {
            auto &window = *i;
            LOGI(TAG, "move window %s", window.title().data());
            windows_.push_back(std::move(window));
            i = pending_windows_.erase(i);
        }
    }

    void LoopOnce() {
        for (auto i = windows_.begin(); i != windows_.end();) {
            auto &window = *i;
            if (window.IsClosed()) {
                LOGI(TAG, "close window %s", window.title().data());
                i = windows_.erase(i);
                continue;
            }
            window.LoopOnce();
            ++i;
        }
    }

public:

    Runtime() {

        runtime = this;

        LOGI(TAG, "ctor");

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        Window::Callback(glfwSetFramebufferSizeCallback, FramebufferSizeCallback);
    }

    ~Runtime() {
        glfwTerminate();
        LOGI(TAG, "dtor");
    }

    void NewWindow(const std::string &title, int width, int height, std::function<void(Window &)> init) {
        LOGI(TAG, "add window %s", title.data());
        auto window = Window(title, width, height);
        auto scope = window.Use();
        init(window);
        pending_windows_.push_back(std::move(window));
    }

    void Loop() {
        while (!windows_.empty() || !pending_windows_.empty()) {
            FPS();
            MoveWindows();
            LoopOnce();
            glfwPollEvents();
        }
    }
};

Runtime *Runtime::runtime = nullptr;
