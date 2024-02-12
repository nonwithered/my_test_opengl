#pragma once

#include "my_player/my_player_controller.h"

#include "my_framework/my_module.h"

template<size_t N>
class LocalPlayerController;

template<>
class LocalPlayerController<0> : public PlayerController {

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

using BaseLocalPlayerController = LocalPlayerController<0>;

template<size_t N> 
class LocalPlayerController : public BaseLocalPlayerController {

private:
    using self_type = LocalPlayerController<N>;

    std::array<Context *, N> window_;

protected:
    LocalPlayerController(std::weak_ptr<Level> level) : BaseLocalPlayerController(level) {
    }

    std::shared_ptr<self_type> controller() {
        return TypeCast<self_type>(BaseLocalPlayerController::self().lock());
    }

public:
    void clear() final {
        window_.fill(nullptr);
    }

    template<size_t i>
    Context *&window() {
        return std::get<i>(window_);
    }

};
