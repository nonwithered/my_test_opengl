#pragma once

#include "my_utils/my_log.h"

#include "my_framework/my_context.h"

class Level;

class PlayerController : public FrameMonitor {

private:

    static constexpr auto TAG = "PlayerController";

    std::weak_ptr<Level> level_;

protected:
    PlayerController(std::weak_ptr<Level> level) : FrameMonitor(), level_(level) {
    }

    std::shared_ptr<Level> data() {
        return level_.lock();
    }

};
