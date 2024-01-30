#pragma once

#include <log.h>

class Module;

class Context {

private:
    Context(const Context &) = delete;

protected:
    Context(Context &&) = default;
    Context() = default;

public:
    virtual ~Context() = default;

    virtual int width() = 0;

    virtual int height() = 0;

    virtual const std::string &title() = 0;

    virtual void Close() = 0;

    virtual void NewModule(std::function<std::unique_ptr<Module>()> create) = 0;

};

class Module {

private:
    static constexpr auto TAG = "Module";
    bool finish_ = false;

private:
    Module(const Module &) = delete;
    Module(Module &&) = delete;

protected:
    Module() = default;

public:

    virtual ~Module() = default;

    virtual void Frame(Context &context) {
    }

    virtual void KeyEvent(Context &context, int key, bool press) {
    }

    void Close() {
        finish_ = true;
    }

    bool IsClosed() {
        return finish_;
    }

};

class Window : public Context {

public:
    static void *Callback(void *, void * = nullptr);

private:

    static constexpr auto TAG = "Window";

    Window(const Window &) = delete;

    GLFWwindow *id_;

    const std::string title_;

    int width_ = 0;
    int height_ = 0;

    std::vector<std::unique_ptr<Module>> modules_;
    std::vector<std::unique_ptr<Module>> pending_modules_;

private:

    void KeyEvent(int key, bool press) {
        auto scope = Use();
        for (auto i = modules_.begin(); i != modules_.end(); ++i) {
            auto &module = *i;
            module->KeyEvent(*this, key, press);
        }
    }

    void Frame() {
        for (auto i = modules_.begin(); i != modules_.end(); ++i) {
            auto &module = *i;
            module->Frame(*this);
        }
    }

    void CleanModules() {
        for (auto i = modules_.begin(); i != modules_.end(); ) {
            auto &module = *i;
            if (module->IsClosed()) {
                i = modules_.erase(i);
                continue;
            }
            ++i;
        }
    }

    void MoveModules() {
        for (auto i = pending_modules_.begin(); i != pending_modules_.end(); ) {
            auto &module = *i;
            LOGI(TAG, "move module");
            modules_.push_back(std::move(module));
            i = pending_modules_.erase(i);
        }
    }

    void SetCallback() {
        glfwSetFramebufferSizeCallback(id_, (GLFWframebuffersizefun) Callback(glfwSetFramebufferSizeCallback));
        glfwSetKeyCallback(id_, (GLFWkeyfun) Callback(glfwSetKeyCallback));
    }

public:

    Window(const std::string &title, int width, int height)
    : id_(glfwCreateWindow(width, height, title.data(), nullptr, nullptr))
    , title_(title)
    , width_(width)
    , height_(height) {
        LOGI(TAG, "ctor %s", title_.data());
        if (id_ == nullptr) {
            throw std::exception();
        }
        auto scope = Use();
        // glfwSwapInterval(0);
        SetCallback();
    }

    Window(Window &&that)
    : id_(that.id_)
    , title_(std::move(that.title_))
    , width_(that.width_)
    , height_(that.height_)
    , modules_(std::move(that.modules_))
    , pending_modules_(std::move(that.pending_modules_)) {
        that.id_ = nullptr;
        that.width_ = 0;
        that.height_ = 0;
    }

    ~Window() {
        if (id_ == nullptr) {
            return;
        }
        LOGI(TAG, "dtor %s", title_.data());
        glfwDestroyWindow(id_);
        id_ = nullptr;
    }

    void LoopOnce() {
        auto scope = Use();
        MoveModules();
        CleanModules();
        Frame();
        glfwSwapBuffers(id_);
    }

    bool IsClosed() {
        return glfwWindowShouldClose(id_);
    }

    bool IsSame(GLFWwindow *id) {
        return id_ == id;
    }

    void FramebufferSizeCallback(int width, int height) {
        LOGI(TAG, "FramebufferSizeCallback %s %d %d", title_.data(), width, height);
        width_ = width;
        height_ = height;
        LoopOnce();
    }

    void KeyCallback(int key, int scancode, int action, int mods) {
        LOGI(TAG, "KeyCallback %s %d %d %d %d", title_.data(), key, scancode, action, mods);
        if (key == GLFW_KEY_UNKNOWN) {
            return;
        }
        if (action != GLFW_PRESS && action != GLFW_RELEASE) {
            return;
        }
        KeyEvent(key, action == GLFW_PRESS);
        LoopOnce();
    }

    class Scope {

    private:

        static bool init;

        static constexpr auto TAG = "Window.Scope";

    private:
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

    public:
        Scope(Window &owner) {
            LOGD(TAG, "bind %p", owner.id_);
            glfwMakeContextCurrent(owner.id_);
            if (init) {
                return;
            }
            init = true;
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                LOGE(TAG, "gladLoadGLLoader fail");
                throw std::exception();
            }
        }
        ~Scope() {
            LOGD(TAG, "unbind");
            glfwMakeContextCurrent(nullptr);
        }
    };

    Scope Use() {
        return Scope(*this);
    }

public:

    int width() override {
        return width_;
    }

    int height() override {
        return height_;
    }

    const std::string &title() override {
        return title_;
    }

    void Close() override {
        glfwSetWindowShouldClose(id_, true);
    }

    void NewModule(std::function<std::unique_ptr<Module>()> create) override {
        pending_modules_.push_back(std::move(create()));
    }
};

bool Window::Scope::init = false;

void *Window::Callback(void *k, void *v) {
    static constexpr auto TAG = "Callback";
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
