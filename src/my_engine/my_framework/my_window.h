#pragma once

#include "my_utils/my_log.h"

#include "my_framework/my_module.h"

class Window : public Context {

    friend class WindowManager;

private:

    static constexpr auto TAG = "Window";

private:

    Window(const Window &) = delete;
    Window(Window &&) = delete;

    GLFWwindow *id_;

    Global &global_;

    bool primary_;
    std::string title_;

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

    GLFWmonitor *GetPrimaryMonitor() {
        auto monitor = glfwGetPrimaryMonitor();
        if (!monitor) {
            LOGE(TAG, "GetPrimaryMonitor fail");
            throw std::exception();
        }
        return monitor;
    }

    GLFWwindow *NewId(const std::string &title, int width, int height) {
        if (width == 0 && height == 0) {
            auto monitor = GetPrimaryMonitor();
            auto mode = glfwGetVideoMode(monitor);
            if (!mode) {
                LOGE(TAG, "GetVideoMode fail");
                throw std::exception();
            }
            width = mode->width;
            height = mode->height;
            LOGI(TAG, "CreateWindow primary %s %d %d", title.data(), width, height);
            return glfwCreateWindow(width, height, title.data(), monitor, nullptr);
        }
        LOGI(TAG, "CreateWindow %s %d %d", title.data(), width, height);
        return glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    }

public:

    Window(
        Global &global,
        const std::string &title,
        int width,
        int height,
        std::function<void(Window &)> init)
    : id_(NewId(title, width, height))
    , global_(global)
    , primary_(width == 0 && height == 0)
    , title_(title) {
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

    GLFWwindow *id() const {
        return id_;
    }

    void PerformFramebufferSize(Module &module, int width, int height) {
        module.PerformFramebufferSize(*this, width, height);

    }

    void PerformKeyEvent(Module &module, int key, bool press) {
        module.PerformKeyEvent(*this, key, press);
    }

    void PerformMouseButtonEvent(Module &module, int button, bool press) {
        module.PerformMouseButtonEvent(*this, button, press);
    }

    void PerformFrame(Module &module) {
        LOGD(TAG, "PerformFrame %s", title_.data());
        auto guard = Use();
        module.PerformFrame(*this);
    }

    void SwapBuffers() {
        glfwSwapBuffers(id_);
    }

    Global &global() override {
        return global_;
    }

    ResourceManager &resource() override {
        return *resource_;
    }

    bool primary() override {
        return primary_;
    }

    const std::string &title() override {
        return title_;
    }

    void SetWindowTitle(const std::string &title) override {
        glfwSetWindowTitle(id_, title.data());
        title_ = title;
    }

    std::array<int, 2> GetFramebufferSize() override {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(id_, &width, &height);
        return { width, height };
    }

    std::array<int, 2> GetWindowSize() override {
        int width = 0;
        int height = 0;
        glfwGetWindowSize(id_, &width, &height);
        return { width, height };
    }

    void SetWindowSize(int width, int height) override {
        glfwSetWindowSize(id_, width, height);
    }

    std::array<int, 2> GetWindowPos() override {
        int x = 0;
        int y = 0;
        glfwGetWindowPos(id_, &x, &y);
        return { x, y };
    }

    void SetWindowPos(int x, int y)  override {
        glfwSetWindowPos(id_, x, y);
    }

    std::array<double, 2> GetCursorPos() override {
        std::array<double, 2> pos{};
        glfwGetCursorPos(id_, &pos[0], &pos[1]);
        return pos;
    }

    void SetCursorPos(double x, double y) override {
        glfwSetCursorPos(id_, x, y);
    }

    bool GetKey(int key) override {
        return glfwGetKey(id_, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    }

    bool GetMouseButton(int button) override {
        return glfwGetMouseButton(id_, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    }

    int GetInputMode(int mode) override {
        return glfwGetInputMode(id_, mode);
    }

    void SetInputMode(int mode, int value) override {
        glfwSetInputMode(id_, mode, value);
    }
};
