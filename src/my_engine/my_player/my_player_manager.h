#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_cast.h"

#include "my_framework/my_context.h"

#include "my_player/my_local_player_controller.h"

class PlayerManager {

public:

    using vector_controller_t = typename std::vector<std::unique_ptr<PlayerController>>;

private:

    static constexpr auto TAG = "Level";

    PlayerManager(const PlayerManager &) = delete;
    PlayerManager(PlayerManager &&) = delete;

    vector_controller_t controller_;

public:

    PlayerManager() = default;
    ~PlayerManager() = default;

    void OnStart(vector_controller_t controller) {
        controller_ = std::move(controller);
    }

    void PerformFrame(Global &context_, std::function<void(Module &)>frame) {
        for (auto &controller : controller_) {
            controller->Frame(context_);
        }
        for (auto &controller : controller_) {
            auto *p = TypeCast<LocalPlayerController>(controller.get());
            if (p) {
                frame(p->module());
            }
        }
    }

    void OnFramebufferSize(Context &context, int width, int height) {
    }

    void PerformKeyEvent(Context &context, int key, bool press) {
        for (auto &controller : controller_) {
            auto *p = TypeCast<LocalPlayerController>(controller.get());
            if (p) {
                p->module().PerformKeyEvent(context, key, press);
            }
        }
    }

    void PerformMouseButtonEvent(Context &context, int button, bool press) {
        for (auto &controller : controller_) {
            auto *p = TypeCast<LocalPlayerController>(controller.get());
            if (p) {
                p->module().PerformMouseButtonEvent(context, button, press);
            }
        }
    }

};
