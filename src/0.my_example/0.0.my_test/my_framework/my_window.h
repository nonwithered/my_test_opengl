#pragma once

#include "my_utils/log.h"

#include "my_framework/my_module.h"

class Window : public Context {

private:

    static constexpr auto TAG = "Window";

public:
    static void *Callback(void *k, void *v = nullptr) {
        static std::unordered_map<void *, void *> m;
        if (k == nullptr) {
            LOGE(TAG, "invalid key");
            throw std::exception();
        }
        auto i = m.find(k);
        if (v != nullptr && i == m.end()) {
            m[k] = v;
            return nullptr;
        }
        if (v == nullptr && i != m.end()) {
            return i->second;
        }
        LOGE(TAG, "Callback invalid %p %p", k, v);
        throw std::exception();
    }

private:

    Window(const Window &) = delete;
    Window(Window &&) = delete;

    GLFWwindow *id_;

    Global &global_;

    const std::string title_;

    int width_ = 0;
    int height_ = 0;

    std::unique_ptr<Module> module_;

    class Scope {

    private:

        static constexpr auto TAG = "Window.Scope";

    private:
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

    public:
        Scope(Window &owner) {
            LOGD(TAG, "bind %p", owner.id_);
            glfwMakeContextCurrent(owner.id_);
        }
        ~Scope() {
            LOGD(TAG, "unbind");
            glfwMakeContextCurrent(nullptr);
        }
    };

    Scope Use() {
        return Scope(*this);
    }

    void Init() {
        glEnable(GL_DEPTH_TEST);
    }

public:

    Window(Global &global, const std::string &title, int width, int height, std::function<std::unique_ptr<Module>(Window &)> launch)
    : id_(glfwCreateWindow(width, height, title.data(), nullptr, nullptr))
    , global_(global)
    , title_(title)
    , width_(width)
    , height_(height) {
        LOGI(TAG, "ctor %s", title_.data());
        if (id_ == nullptr) {
            throw std::exception();
        }
        auto scope = Use();
        if (!launch) {
            LOGW(TAG, "invalid launch");
            throw std::exception();
        }
        module_ = launch(*this);
        Init();
    }

    ~Window() {
        if (id_ == nullptr) {
            return;
        }
        LOGI(TAG, "dtor %s", title_.data());
        glfwDestroyWindow(id_);
        id_ = nullptr;
    }

    bool PerformFrame() {
        LOGD(TAG, "PerformFrame %s", title_.data());
        auto scope = Use();
        if (!module_->PerformFrame()) {
            glfwSwapBuffers(id_);
            return false;
        } else {
            module_.reset();
            return true;
        }
    }

    bool IsSame(GLFWwindow *id) {
        return id_ == id;
    }

    void FramebufferSizeCallback(int width, int height) {
        LOGI(TAG, "FramebufferSizeCallback %s %d %d", title_.data(), width, height);
        width_ = width;
        height_ = height;
    }

    void KeyCallback(int key, int scancode, int action, int mods) {
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
            auto scope = Use();
            module_->PerformKeyEvent(key, press);
        }
    }

    void MouseButtonCallback(int button, int action, int mods) {
        LOGD(TAG, "MouseButtonCallback %s %d %d %d", title_.data(), button, action, mods);
        if (action != GLFW_PRESS && action != GLFW_RELEASE) {
            return;
        }
        bool press = action == GLFW_PRESS;
        LOGI(TAG, "PerformMouseButtonEvent %s %d %d", title_.data(), button, press);
        {
            auto scope = Use();
            module_->PerformMouseButtonEvent(button, press);
        }

    }

    GLFWwindow *id() {
        return id_;
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

    void NewModule(std::unique_ptr<Module> module) override {
        LOGI(TAG, "NewModule");
        module_->NewModule(std::move(module));
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
