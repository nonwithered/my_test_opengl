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

    RectangleModule(std::function<DrawRectangle()> draw) : Module(), draw_(draw()) {
        LOGI(TAG, "ctor");
    }

    ~RectangleModule() override {
        LOGI(TAG, "dtor");
    }

    void Frame(Context &context) override {
        auto width = right_ - left_;
        auto height = bottom_ - top_;
        auto x = left_;
        auto y = context.height() - height - top_;
        draw_(x, y, width, height);
    }

    void Events(Context &context) override {
        auto interval = counter_.Count();
        auto step = interval * step_;
        auto delta_rate = 100 * interval;
        auto min_rate = 100;
        if (context.GetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
            left_ -= step;
            right_ -= step;
        }
        if (context.GetKey(GLFW_KEY_UP) == GLFW_PRESS) {
            top_ -= step;
            bottom_ -= step;
        }
        if (context.GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
            left_ += step;
            right_ += step;
        }
        if (context.GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
            top_ += step;
            bottom_ += step;
        }
        if (context.GetKey(GLFW_KEY_A) == GLFW_PRESS) {
            step_ += delta_rate;
        }
        if (context.GetKey(GLFW_KEY_S) == GLFW_PRESS) {
            step_ -= delta_rate;
            if (step_ < min_rate) {
                step_ = min_rate;
            }
        }
        if (context.GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
            left_ = reset_left_;
            top_ = reset_top_;
            right_ = reset_right_;
            bottom_ = reset_bottom_;
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
        glViewport(0, 0, context.width(), context.height());
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Events(Context &context) override {
        if(context.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            context.Close();
        }
        if (context.GetKey(GLFW_KEY_1) == GLFW_PRESS) {
            if (!rect_created) {
                rect_created = true;
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
                });
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
                });
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
                });
            }
        }
    }
};
