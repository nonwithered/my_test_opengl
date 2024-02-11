#pragma once

#include "my_utils/log.h"

#include "my_framework/my_module.h"

class Window : public Context {

private:

    static constexpr auto TAG = "Window";

private:

    Window(const Window &) = delete;
    Window(Window &&) = delete;

    GLFWwindow *id_;

    Global &global_;

    const std::string title_;

    int width_ = 0;
    int height_ = 0;

    std::unique_ptr<ResourceManager> resource_ = std::make_unique<ResourceManager>();

    class Guard {

    private:

        static constexpr auto TAG = "Window.Guard";

    private:
        Guard(const Guard &) = delete;
        Guard(Guard &&) = delete;

    public:
        Guard(Window &owner) {
            LOGD(TAG, "bind %p", owner.id_);
            glfwMakeContextCurrent(owner.id_);
        }
        ~Guard() {
            LOGD(TAG, "unbind");
            glfwMakeContextCurrent(nullptr);
        }
    };

    Guard Use() {
        return Guard(*this);
    }

public:

    Window(
        Global &global,
        const std::string &title,
        int width,
        int height,
        std::function<void(Window &)> init)
    : id_(glfwCreateWindow(width, height, title.data(), nullptr, nullptr))
    , global_(global)
    , title_(title)
    , width_(width)
    , height_(height) {
        LOGI(TAG, "ctor %s", title_.data());
        if (id_ == nullptr) {
            throw std::exception();
        }
        auto guard = Use();
        if (!init) {
            LOGW(TAG, "invalid module");
            throw std::exception();
        }
        init(*this);
    }

    ~Window() {
        if (id_ == nullptr) {
            return;
        }
        LOGI(TAG, "dtor %s", title_.data());
        {
            auto guard = Use();
            resource_.reset();
        }
        glfwDestroyWindow(id_);
        id_ = nullptr;
    }

    bool IsSame(GLFWwindow *id) {
        return id_ == id;
    }

    void SetFramebufferSizeCallback(GLFWframebuffersizefun callback) {
        glfwSetFramebufferSizeCallback(id_, callback);
    }

    void SetKeyCallback(GLFWkeyfun callback) {
        glfwSetKeyCallback(id_, callback);
    }

    void SetMouseButtonCallback(GLFWmousebuttonfun callback) {
        glfwSetMouseButtonCallback(id_, callback);
    }

    void FramebufferSizeCallback(int width, int height) {
        LOGI(TAG, "FramebufferSizeCallback %s %d %d", title_.data(), width, height);
        width_ = width;
        height_ = height;
    }

    void KeyCallback(Module &module, int key, int scancode, int action, int mods) {
        LOGD(TAG, "KeyCallback %s %d %d %d %d", title_.data(), key, scancode, action, mods);
        if (key == GLFW_KEY_UNKNOWN) {
            return;
        }
        if (action != GLFW_PRESS && action != GLFW_RELEASE) {
            return;
        }
        bool press = action == GLFW_PRESS;
        LOGI(TAG, "PerformKeyEvent %s %d %d", title_.data(), key, press);
        {
            auto guard = Use();
            module.PerformKeyEvent(*this, key, press);
        }
    }

    void MouseButtonCallback(Module &module, int button, int action, int mods) {
        LOGD(TAG, "MouseButtonCallback %s %d %d %d", title_.data(), button, action, mods);
        if (action != GLFW_PRESS && action != GLFW_RELEASE) {
            return;
        }
        bool press = action == GLFW_PRESS;
        LOGI(TAG, "PerformMouseButtonEvent %s %d %d", title_.data(), button, press);
        {
            auto guard = Use();
            module.PerformMouseButtonEvent(*this, button, press);
        }

    }

    void PerformFrame(Module &module) {
        LOGD(TAG, "PerformFrame %s", title_.data());
        auto guard = Use();
        if (!module.PerformFrame(*this)) {
            LOGD(TAG, "SwapBuffers %s", title_.data());
            glfwSwapBuffers(id_);
        } else {
            LOGD(TAG, "ignore SwapBuffers %s", title_.data());
        }
    }

    Global &global() override {
        return global_;
    }

    int width() override {
        return width_;
    }

    int height() override {
        return height_;
    }

    const std::string &title() override {
        return title_;
    }

    ResourceManager &resource() override {
        return *resource_;
    }

    bool GetKey(int key) override {
        return glfwGetKey(id_, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    }

    bool GetMouseButton(int button) override {
        return glfwGetMouseButton(id_, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    }

    std::array<double, 2> GetCursorPos() override {
        std::array<double, 2> pos{};
        glfwGetCursorPos(id_, &pos[0], &pos[1]);
        return pos;
    }
};
