#pragma once

#include "my_player/my_player_controller.h"

#include "my_framework/my_module.h"

#include "my_model/my_camera_component.h"

template<size_t W, size_t C>
class LocalPlayerController;

template<>
class LocalPlayerController<0, 0> : public PlayerController {

private:

    static constexpr auto TAG = "LocalPlayerController";
    
    Module module_;

protected:
    LocalPlayerController(std::weak_ptr<Level> level) : PlayerController(level) {
    }

public:

    Module &module() {
        return module_;
    }

    virtual void OnWindowClose(Context &context) {
    }

    virtual void clear() {
    }

};

using BaseLocalPlayerController = LocalPlayerController<0, 0>;

template<size_t W, size_t C> 
class LocalPlayerController : public BaseLocalPlayerController {

private:
    using self_type = LocalPlayerController<W, C>;

    std::array<Context *, W> window_;
    std::array<std::shared_ptr<CameraComponent>, C> camera_;

protected:
    LocalPlayerController(std::weak_ptr<Level> level) : BaseLocalPlayerController(level) {
    }

    std::shared_ptr<self_type> controller() {
        return TypeCast<self_type>(BaseLocalPlayerController::self().lock());
    }

    void OnPause() override {
        BaseLocalPlayerController::OnPause();
        window_.fill(nullptr);
    }

public:

    template<size_t i>
    Context *&window() {
        return std::get<i>(window_);
    }

    template<size_t i>
    std::shared_ptr<CameraComponent> &camera() {
        return std::get<i>(camera_);
    }

};
