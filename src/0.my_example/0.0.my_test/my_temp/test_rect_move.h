#pragma once

#include "my_framework/my_runtime.h"

#include "my_shape/my_rectangle.h"

bool rect_created = false;

class RectangleModule : public Module {

private:
    static constexpr auto TAG = "RectangleModule";

    static constexpr auto kRate = 100;

    DrawRectangle draw_;

    float step_ = kRate;

    bool move_left_ = false;
    bool move_up_ = false;
    bool move_right_ = false;
    bool move_down_ = false;

    bool move_fast_ = false;
    bool move_slow_ = false;

public:

    float reset_left_ = 0;
    float reset_top_ = 0;
    float reset_right_ = 0;
    float reset_bottom_ = 0;

    float left_ = 0;
    float top_ = 0;
    float right_ = 0;
    float bottom_ = 0;

    RectangleModule(Context &context, std::function<DrawRectangle()> draw) : Module(context), draw_([&draw]() -> auto {
        if (draw) {
            return draw();
        } else {
            LOGE(TAG, "invalid init");
            throw std::exception();
        }
    }()) {
        LOGI(TAG, "ctor");
        SetupEvent();
    }

    ~RectangleModule() override {
        LOGI(TAG, "dtor");
    }

    void Frame(float fraction) override {
        Change(fraction);
        auto width = right_ - left_;
        auto height = bottom_ - top_;
        auto x = left_;
        auto y = context().height() - height - top_;
        draw_(x, y, width, height);
        Module::Frame(fraction);
    }

private:

    void BindFlag(int key, bool RectangleModule:: *flag) {
        Module::KeyEvent(key, true, [this, flag]() {
            this->*flag = true;
            return false;
        });
        Module::KeyEvent(key, false, [this, flag]() {
            this->*flag = false;
            return false;
        });
    }

    void SetupEvent() {
        Module::KeyEvent(GLFW_KEY_2, true, [this]() {
            Close();
            return false;
        });
        Module::KeyEvent(GLFW_KEY_SPACE, true, [this]() {
            left_ = reset_left_;
            top_ = reset_top_;
            right_ = reset_right_;
            bottom_ = reset_bottom_;
            return false;
        });
        BindFlag(GLFW_KEY_LEFT, &RectangleModule::move_left_);
        BindFlag(GLFW_KEY_UP, &RectangleModule::move_up_);
        BindFlag(GLFW_KEY_RIGHT, &RectangleModule::move_right_);
        BindFlag(GLFW_KEY_DOWN, &RectangleModule::move_down_);
        BindFlag(GLFW_KEY_A, &RectangleModule::move_fast_);
        BindFlag(GLFW_KEY_S, &RectangleModule::move_slow_);
    }

    void Change(float fraction) {
        auto step = fraction * step_;
        auto delta_rate = kRate * fraction;
        auto min_rate = kRate;
        if (move_left_) {
            left_ -= step;
            right_ -= step;
        }
        if (move_up_) {
            top_ -= step;
            bottom_ -= step;
        }
        if (move_right_) {
            left_ += step;
            right_ += step;
        }
        if (move_down_) {
            top_ += step;
            bottom_ += step;
        }
        if (move_fast_) {
            step_ += delta_rate;
        }
        if (move_slow_) {
            step_ -= delta_rate;
            if (step_ < min_rate) {
                step_ = min_rate;
            }
        }
    }
};

class BackgroundModule : public Module {

private:
    static constexpr auto TAG = "BackgroundModule";

    static constexpr auto kRate = 0.1;

public:

    float r = 0.5;
    float g = 0.5;
    float b = 0.5;

    bool r_ = false;
    bool g_ = false;
    bool b_ = false;

    bool p_ = false;

    BackgroundModule(Context &context) : Module(context) {
        LOGI(TAG, "ctor");
        SetupEvent();
    }

    ~BackgroundModule() override {
        LOGI(TAG, "dtor");
    }

    void Frame(float fraction) override {
        Change(fraction);
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Module::Frame(fraction);
    }

private:

    void SetupEvent() {
        Module::KeyEvent(GLFW_KEY_ESCAPE, true, [this]() {
            context().Close();
            return false;
        });
        Module::KeyEvent(GLFW_KEY_1, true, [this]() {
            if (!rect_created) {
                rect_created = true;
                InitSubModule();
            }
            return false;
        });
        BindFlag(GLFW_KEY_Z, &BackgroundModule::r_);
        BindFlag(GLFW_KEY_X, &BackgroundModule::g_);
        BindFlag(GLFW_KEY_C, &BackgroundModule::b_);
        BindFlag(GLFW_KEY_V, &BackgroundModule::p_);
    }

    void BindFlag(int key, bool BackgroundModule:: *flag) {
        Module::KeyEvent(key, true, [this, flag]() {
            this->*flag = true;
            return false;
        });
        Module::KeyEvent(key, false, [this, flag]() {
            this->*flag = false;
            return false;
        });
    }

    void Change(float fraction) {
        auto step = fraction * kRate;
        if (p_) {
            step *= -1;
        }
        if (r_) {
            r += step;
            if (r > 1) {
                r = 1;
            }
            if (r < 0) {
                r = 0;
            }
        }
        if (g_) {
            g += step;
            if (g > 1) {
                g = 1;
            }
            if (g < 0) {
                g = 0;
            }
        }
        if (b_) {
            b += step;
            if (b > 1) {
                b = 1;
            }
            if (b < 0) {
                b = 0;
            }
        }
    }

    void InitSubModule() {
        Module::NewModule([this]() -> std::unique_ptr<Module> {
            auto module = std::make_unique<RectangleModule>(context(), []() -> auto {
                return DrawRectangleSingleColor([]() { return 0xffff00ff; });
            });

            module->reset_left_ = 200;
            module->reset_top_ = 100;
            module->reset_right_ = 300;
            module->reset_bottom_ = 300;

            module->left_ = module->reset_left_;
            module->top_ = module->reset_top_;
            module->right_ = module->reset_right_;
            module->bottom_ = module->reset_bottom_;

            return module;
        }());
        Module::NewModule([this]() -> std::unique_ptr<Module> {
            auto module = std::make_unique<RectangleModule>(context(), []() -> auto {
                return DrawRectangleMultiColor({ 0xffff0000, 0xff00ff00, 0xff0000ff, 0xffffffff });
            });

            module->reset_left_ = 400;
            module->reset_top_ = 100;
            module->reset_right_ = 500;
            module->reset_bottom_ = 300;

            module->left_ = module->reset_left_;
            module->top_ = module->reset_top_;
            module->right_ = module->reset_right_;
            module->bottom_ = module->reset_bottom_;

            return module;
        }());
        Module::NewModule([this]() -> std::unique_ptr<Module> {
            auto module = std::make_unique<RectangleModule>(context(), []() -> auto {
                return DrawRectangleWithPicture("resources/textures/container.jpg");
            });

            module->reset_left_ = 600;
            module->reset_top_ = 100;
            module->reset_right_ = 700;
            module->reset_bottom_ = 300;

            module->left_ = module->reset_left_;
            module->top_ = module->reset_top_;
            module->right_ = module->reset_right_;
            module->bottom_ = module->reset_bottom_;

            return module;
        }());
    }
};



class LauncherModule : public Module {

private:
    static constexpr auto TAG = "LauncherModule";

public:
    LauncherModule(Context &context) : Module(context) {
        context.NewModule(std::make_unique<BackgroundModule>(context));
    }
};
