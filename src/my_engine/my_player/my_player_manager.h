#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_cast.h"

#include "my_framework/my_context.h"

#include "my_player/my_local_player_controller.h"

class PlayerManager {

public:
    
    template<typename T>
    T *FindPlayer(std::function<bool(T &)> block = nullptr) {
        for (auto &controller : controller_) {
            T *p = TypeCast<T>(controller.get());
            if (p && (!block || block(*p))) {
                return p;
            }
        }
        return nullptr;
    }

    PlayerManager() = default;
    ~PlayerManager() = default;

protected:

    using controller_t = typename std::shared_ptr<PlayerController>;
    using vector_controller_t = typename std::vector<controller_t>;

    static constexpr auto TAG = "Level";

    PlayerManager(const PlayerManager &) = delete;
    PlayerManager(PlayerManager &&) = delete;

    vector_controller_t controller_;

    void NewPlayer(controller_t controller) {
        controller_.push_back(std::move(controller));
    }

    void Collect(std::function<void(PlayerController &)> block) {
        for (auto &controller : controller_) {
            block(*controller);
        }
    }

    bool PerformFrame(Global &context_, std::function<void(Module &)>frame) {
        for (auto i = controller_.begin(); i != controller_.end(); ) {
            auto &controller = *i;
            if (controller->quit_) {
                i = controller_.erase(i);
            } else {
                controller->Frame(context_);
                ++i;
            }
        }
        bool exist = false;
        for (auto &controller : controller_) {
            auto *p = TypeCast<BaseLocalPlayerController>(controller.get());
            if (p) {
                exist = true;
                frame(p->module());
            }
        }
        return !exist;
    }

    void PerformKeyEvent(Context &context, int key, bool press) {
        for (auto &controller : controller_) {
            auto *p = TypeCast<BaseLocalPlayerController>(controller.get());
            if (p) {
                p->module().PerformKeyEvent(context, key, press);
            }
        }
    }

    void PerformMouseButtonEvent(Context &context, int button, bool press) {
        for (auto &controller : controller_) {
            auto *p = TypeCast<BaseLocalPlayerController>(controller.get());
            if (p) {
                p->module().PerformMouseButtonEvent(context, button, press);
            }
        }
    }

    void OnWindowClose(Context &context) {
        for (auto &controller : controller_) {
            auto *p = TypeCast<BaseLocalPlayerController>(controller.get());
            if (p) {
                p->OnWindowClose(context);
            }
        }
    }
};
