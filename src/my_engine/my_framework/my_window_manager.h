#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_contains.h"

#include "my_framework/my_module.h"
#include "my_framework/my_window.h"

class WindowPresenter {

private:
    WindowPresenter(const WindowPresenter &) = delete;
    WindowPresenter(WindowPresenter &&) = delete;

protected:
    WindowPresenter() = default;

public:
    virtual ~WindowPresenter() = default;

    virtual operator Global &() = 0;

    virtual void OnFramebufferSize(Window &window, int width, int height) = 0;
    virtual void PerformKeyEvent(Window &window, int key, bool press) = 0;
    virtual void PerformMouseButtonEvent(Window &window, int button, bool press) = 0;
    virtual void OnWindowClose(Window &window) = 0;

};

class WindowManager {

    friend class Runtime;
    friend class Level;

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

    std::unique_ptr<Window> primary_;
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
            auto *w = manager.Find(window);
            if (!w) {
                LOGW(TAG, "SetFramebufferSizeCallback but window not found");
                return;
            }
            auto &context = *w;
            LOGD(TAG, "FramebufferSizeCallback %s %d %d", context.title().data(), width, height);
            manager.presenter_.OnFramebufferSize(context, width, height);
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
            auto *w = manager.Find(window);
            if (!w) {
                LOGW(TAG, "SetKeyCallback but window not found");
                return;
            }
            auto &context = *w;
            LOGD(TAG, "KeyCallback %s %d %d %d %d", context.title().data(), key, scancode, action, mods);
            manager.presenter_.PerformKeyEvent(context, key, press);
        });
        glfwSetMouseButtonCallback(w.id(), [](GLFWwindow *window, int button, int action, int mods) {
            if (action != GLFW_PRESS && action != GLFW_RELEASE) {
                return;
            }
            auto press = action == GLFW_PRESS;
            auto &manager = Instance();
            auto *w = manager.Find(window);
            if (!w) {
                LOGW(TAG, "SetMouseButtonCallback but window not found");
                return;
            }
            auto &context = *w;
            LOGD(TAG, "MouseButtonCallback %s %d %d %d", context.title().data(), button, action, mods);
            manager.presenter_.PerformMouseButtonEvent(context, button, press);
        });
        glfwSetWindowCloseCallback(w.id(), [](GLFWwindow *window) {
            auto &manager = Instance();
            auto *w = manager.Find(window);
            if (!w) {
                return;
            }
            auto &context = *w;
            LOGI(TAG, "WindowCloseCallback %s", context.title().data());
            manager.presenter_.OnWindowClose(context);
        });
        // glfwSwapInterval(0);
        glEnable(GL_DEPTH_TEST);
    }

    void PerformFrame(Module &module) {
        if (primary_) {
            primary_->PerformFrame(module);
        }
        MovePending();
        for (auto &window : windows_) {
            window->PerformFrame(module);
        }
    }

    void OnWindowClose(Window &window) {
        LOGI(TAG, "OnWindowClose %s", window.title().data());
        if (primary_.get() == &window) {
            primary_.reset();
        }
        MovePending();
        for (auto i = windows_.begin(); i != windows_.end(); ) {
            auto &p = *i;
            if (p.get() == &window) {
                i = windows_.erase(i);
            } else {
                ++i;
            }
        }
    }

    void SwapBuffers() {
        if (primary_) {
            primary_->SwapBuffers();
        }
        for (auto &window : windows_) {
            window->SwapBuffers();
        }
    }

    Window *Find(GLFWwindow *id) {
        if (primary_) {
            auto window = primary_.get();
            if (window->id() == id) {
                return window;
            }
        }
        for (auto &window : windows_) {
            if (window->id() == id) {
                return window.get();
            }
        }
        return nullptr;
    }

    void MovePending() {
        for (auto i = pending_windows_.begin(); i != pending_windows_.end(); ) {
            auto &window = *i;
            LOGI(TAG, "move window %s", window->title().data());
            windows_.push_back(std::move(window));
            i = pending_windows_.erase(i);
        }
    }

    Context &RequirePrimary(const std::string &title) {
        Context *context = nullptr;
        if (primary_) {
            context = primary_.get();
            context->SetWindowTitle(title);
        } else {
            context = &NewWindow(title);
        }
        return *context;
    }

    Context &RequireIndex(size_t index, const std::string &title, int width, int height) {
        Context *context = nullptr;
        auto size_ = windows_.size();
        if (size_ > index) {
            context = windows_[index].get();
            context->SetWindowTitle(title);
            context->SetWindowSize(width, height);
        } else if (size_ == index) {
            context = &NewWindow(title, width, height);
        } else {
            LOGE(TAG, "RequireIndex fail %" PRIu64 " %" PRIu64, (uint64_t) index, (uint64_t) size_);
            throw std::exception();
        }
        return *context;
    }

    void EnsureSurvivor(std::vector<Context *> windows) {
        for (auto context : windows) {
            auto window = TypeCast<Window>(context);
            if (context && !window) {
                LOGE(TAG, "EnsureSurvivor fail");
                throw std::exception();
            }
        }
        if (!Contains(windows, (Context *) primary_.get())) {
            primary_.reset();
        }
        MovePending();
        for (auto i = windows_.begin(); i != windows_.end(); ) {
            auto &window = *i;
            if (!Contains(windows, (Context *) window.get())) {
                i = windows_.erase(i);
            } else {
                ++i;
            }
        }
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

        primary_.reset();
        windows_.clear();
        pending_windows_.clear();
        glfwTerminate();
        init_ = false;
    }

    Context &NewWindow(const std::string &title, int width = 0, int height = 0) {
        LOGI(TAG, "NewWindow %s %d %d", title.data(), width, height);
        bool primary = width == 0 && height == 0;
        if (primary_ && primary) {
            LOGW(TAG, "NewWindow primary duplicate");
            return *primary_;
        }
        auto window = std::make_unique<Window>(presenter_, title, width, height, [this](Window &w) {
            SetupWindow(w);
        });
        auto &context = *window;
        if (primary) {
            primary_ = std::move(window);
        } else {
            pending_windows_.push_back(std::move(window));
        }
        return context;
    }

};
