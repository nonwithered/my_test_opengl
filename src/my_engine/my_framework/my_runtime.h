#pragma once

#include "my_utils/log.h"

#include "my_framework/my_window.h"

#include "my_utils/my_counter.h"
#include "my_utils/my_interval.h"
#include "my_model/my_actor.h"

class Runtime : public Global {

private:

    static constexpr auto TAG = "Runtime";

    bool init_gl_ = false;

    std::vector<std::unique_ptr<Window>> windows_;
    std::vector<std::unique_ptr<Window>> pending_windows_;

    std::shared_ptr<Actor> actor_ = Actor::Make();

    Counter counter_ = Counter(1);
    Interval interval_ = Interval(1);

    float interval_fraction_ = 0.0f;

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
        for (auto i = windows_.begin(); i != windows_.end();) {
            auto &window = *i;
            if (!window->PerformFrame()) {
                ++i;
            } else {
                LOGI(TAG, "close window %s", window->title().data());
                i = windows_.erase(i);
            }
        }
        return windows_.empty() && pending_windows_.empty();
    }

    void SetupWindow(Window &w) {
        if (!init_gl_) {
            init_gl_ = true;
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                LOGE(TAG, "gladLoadGLLoader fail");
                throw std::exception();
            }
        }
        glfwSetFramebufferSizeCallback(w.id(), [](GLFWwindow *window, int width, int height) {
            auto &runtime = *Instance();
            runtime.Find(window).FramebufferSizeCallback(width, height);
            runtime.PerformFrame();
        });
        glfwSetKeyCallback(w.id(), [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto &runtime = *Instance();
            runtime.Find(window).KeyCallback(key, scancode, action, mods);
            runtime.PerformFrame();
        });
        glfwSetMouseButtonCallback(w.id(), [](GLFWwindow *window, int button, int action, int mods) {
            auto &runtime = *Instance();
            runtime.Find(window).MouseButtonCallback(button, action, mods);
            runtime.PerformFrame();
        });
        // glfwSwapInterval(0);
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
    }

    ~Runtime() {
        glfwTerminate();
        LOGI(TAG, "dtor");
    }

    void NewWindow(const std::string &title, int width, int height, std::function<std::unique_ptr<Module>(Context &)> launch) {
        LOGI(TAG, "NewWindow %s", title.data());
        if (!launch) {
            LOGE(TAG, "NewWindow invalid launch");
            throw std::exception();
        }
        auto window = std::make_unique<Window>(*this, title, width, height, [this, &launch](Window &w) {
            SetupWindow(w);
            return launch(w);
        });
        pending_windows_.push_back(std::move(window));
    }

    void Loop() {
        while (!PerformFrame()) {
            glfwPollEvents();
        }
    }

    Actor &model() override {
        return *actor_;
    }

    float interval() override {
        return interval_fraction_;
    }
};
