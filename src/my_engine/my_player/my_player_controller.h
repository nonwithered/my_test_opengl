#pragma once

#include "my_utils/my_log.h"

#include "my_framework/my_context.h"

class Level;

class PlayerController : public Model<PlayerController>, public FrameMonitor {

    friend class PlayerManager;

private:

    static constexpr auto TAG = "PlayerController";

    std::weak_ptr<Level> level_;

    bool quit_ = false;

    PlayerController(const PlayerController &) = delete;
    PlayerController(PlayerController &&) = delete;

protected:
    PlayerController(std::weak_ptr<Level> level) : FrameMonitor(), level_(level) {
    }

    virtual void OnResume() {
    }

    virtual void OnPause() {
    }

public:

    virtual ~PlayerController() = default;

    std::shared_ptr<Level> level() {
        return level_.lock();
    }

    void Quit() {
        if (quit_) {
            LOGW(TAG, "Quit duplicate");
            return;
        }
        LOGI(TAG, "Quit");
        quit_ = true;
    }

};
