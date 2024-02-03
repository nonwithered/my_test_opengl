#pragma once

#include "my_framework/my_runtime.h"

#include "my_shape/my_rectangle.h"

#include "my_utils/my_interval.h"

bool rect_created = false;

class RectangleModule : public Module {

private:
    static constexpr auto TAG = "RectangleModule";

    DrawRectangle draw_;

    float step_ = 100;

    IntervalCounter counter_;

public:

    float reset_left_ = 0;
    float reset_top_ = 0;
    float reset_right_ = 0;
    float reset_bottom_ = 0;

    float left_ = 0;
    float top_ = 0;
    float right_ = 0;
    float bottom_ = 0;

    bool move_left_ = false;
    bool move_up_ = false;
    bool move_right_ = false;
    bool move_down_ = false;

    bool move_fast_ = false;
    bool move_slow_ = false;

    RectangleModule(std::function<DrawRectangle()> draw) : Module(), draw_(draw()) {
        LOGI(TAG, "ctor");
    }

    ~RectangleModule() override {
        LOGI(TAG, "dtor");
    }

    void Frame(Context &context) override {
        PerformMove();
        auto width = right_ - left_;
        auto height = bottom_ - top_;
        auto x = left_;
        auto y = context.height() - height - top_;
        draw_(x, y, width, height);
    }

    void Event(Context &context, int key, bool press) override {
        switch (key) {
            case GLFW_KEY_2: {
                if (press) {
                    Close();
                }
                break;
            }
            case GLFW_KEY_LEFT: {
                move_left_ = press;
                break;
            }
            case GLFW_KEY_UP: {
                move_up_ = press;
                break;
            }
            case GLFW_KEY_RIGHT: {
                move_right_ = press;
                break;
            }
            case GLFW_KEY_DOWN: {
                move_down_ = press;
                break;
            }
            case GLFW_KEY_A: {
                move_fast_ = press;
                break;
            }
            case GLFW_KEY_S: {
                move_slow_ = press;
                break;
            }
            case GLFW_KEY_SPACE: {
                left_ = reset_left_;
                top_ = reset_top_;
                right_ = reset_right_;
                bottom_ = reset_bottom_;
                break;
            }
        }
    }

private:

    void PerformMove() {
        auto interval = counter_.Count();
        auto step = interval * step_;
        auto delta_rate = 100 * interval;
        auto min_rate = 100;
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

public:
    BackgroundModule() : Module() {
        LOGI(TAG, "ctor");
    }

    ~BackgroundModule() override {
        LOGI(TAG, "dtor");
    }

    void Frame(Context &context) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Event(Context &context, int key, bool press) override {
        if (!press) {
            return;
        }
        switch (key) {
            case GLFW_KEY_ESCAPE: {
                context.Close();
                break;
            }
            case GLFW_KEY_1: {
                if (rect_created) {
                    break;
                }
                rect_created = true;
                InitSubModule(context);
                break;
            }
        }
    }

private:

    void InitSubModule(Context &context) {
        context.NewModule([]() -> std::unique_ptr<Module> {
            auto module = std::make_unique<RectangleModule>([]() -> auto {
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
        context.NewModule([]() -> std::unique_ptr<Module> {
            auto module = std::make_unique<RectangleModule>([]() -> auto {
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
        context.NewModule([]() -> std::unique_ptr<Module> {
            auto module = std::make_unique<RectangleModule>([]() -> auto {
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
