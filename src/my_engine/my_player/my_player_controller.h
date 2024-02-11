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

protected:
    PlayerController(std::weak_ptr<Level> level) : FrameMonitor(), level_(level) {
    }

public:

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
