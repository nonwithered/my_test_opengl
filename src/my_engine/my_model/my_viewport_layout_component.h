#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_cast.h"

#include "my_model/my_actor.h"

class ViewportPresenter {

private:
    ViewportPresenter(const ViewportPresenter &) = delete;
    ViewportPresenter(ViewportPresenter &&) = delete;

    std::array<int, 4> port_ = { 0, 0, 0, 0, };

protected:
    ViewportPresenter() = default;
    virtual ~ViewportPresenter() = default;

public:

    std::array<int, 4> port() const {
        return port_;
    }

    virtual void port(const std::array<int, 4> &v) {
        port_ = v;
    }

};

class ViewportLayoutComponent : public Actor {

public:
    enum class AlignHorizontal {
        CENTER, LEFT, RIGHT,
    };
    enum class AlignVertical {
        CENTER, TOP, BOTTOM,
    };
    enum class MeasurePrimary {
        NONE, WIDTH, HEIGHT,
    };

private:
    PROPERTY(AlignHorizontal, align_horizontal);
    PROPERTY(AlignVertical, align_vertical);
    PROPERTY(MeasurePrimary, measure_primary);
    PROPERTY(float, margin_horizontal);
    PROPERTY(float, margin_vertical);
    PROPERTY(float, size_width);
    PROPERTY(float, size_height);

public:
    void SetupViewport(int width, int height) {
        auto viewport = TypeCast<ViewportPresenter>(parent());
        if (!viewport) {
            return;
        }

        if (width <= 0 || height <= 0) {
            return;
        }

        float container_left = 0.0f;
        float container_right = (float) width;
        switch (align_horizontal_) {
            case AlignHorizontal::CENTER: {
                container_left += margin_horizontal_;
                container_right -= margin_horizontal_;
                break;
            }
            case AlignHorizontal::LEFT: {
                container_left += margin_horizontal_;
                break;
            }
            case AlignHorizontal::RIGHT: {
                container_right -= margin_horizontal_;
                break;
            }
        }

        float container_bottom = 0.0f;
        float container_top = (float) height;
        switch (align_vertical_) {
            case AlignVertical::CENTER: {
                container_bottom += margin_vertical_;
                container_top -= margin_vertical_;
                break;
            }
            case AlignVertical::TOP: {
                container_top -= margin_vertical_;
                break;
            }
            case AlignVertical::BOTTOM: {
                container_bottom += margin_vertical_;
                break;
            }
        }

        auto measured_width = 0.0f;
        if (measure_primary_ != MeasurePrimary::HEIGHT) {
            if (size_width_ > 0) {
                measured_width = size_width_;
            } else {
                measured_width = container_right - container_left + size_width_;
            }
        }
        
        auto measured_height = 0.0f;
        if (measure_primary_ != MeasurePrimary::WIDTH) {
            if (size_height_ > 0) {
                measured_height = size_height_;
            } else {
                measured_height = container_top - container_bottom + size_height_;
            }
        }

        if (measure_primary_ == MeasurePrimary::HEIGHT) {
            measured_width = measured_height * size_width_;
        }

        if (measure_primary_ == MeasurePrimary::WIDTH) {
            measured_height = measured_width * size_height_;
        }

        auto layout_left = container_left;
        auto layout_right = container_right;
        switch (align_horizontal_) {
            case AlignHorizontal::CENTER: {
                auto horizontal_center = (container_left + container_right) / 2;
                auto half_width = measured_width / 2;
                layout_left = horizontal_center - half_width;
                layout_right = horizontal_center + half_width;
                break;
            }
            case AlignHorizontal::LEFT: {
                layout_right = layout_left + measured_width;
                break;
            }
            case AlignHorizontal::RIGHT: {
                layout_left = layout_right - measured_width;
                break;
            }
        }

        auto layout_top = container_top;
        auto layout_bottom = container_bottom;
        switch (align_vertical_) {
            case AlignVertical::CENTER: {
                auto vertical_center = (container_top + container_bottom) / 2;
                auto half_height = measured_height / 2;
                layout_top = vertical_center + half_height;
                layout_bottom = vertical_center - half_height;
                break;
            }
            case AlignVertical::TOP: {
                layout_bottom = layout_top - measured_height;
                break;
            }
            case AlignVertical::BOTTOM: {
                layout_top = layout_bottom + measured_height;
                break;
            }
        }

        int port_x = (int) layout_left;
        int port_y = (int) layout_bottom;
        int port_w = (int) (layout_right - layout_left);
        int port_h = (int) (layout_top - layout_bottom);

        LOGW("test", "%d %d %d %d", port_x, port_y, port_w, port_h);
        viewport->port({ port_x, port_y, port_w, port_h, });
    }

};
