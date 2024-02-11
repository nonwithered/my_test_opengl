#pragma once

#include "my_player/my_player_controller.h"

class LocalPlayerController : PlayerController {

private:

    static constexpr auto TAG = "LocalPlayerController";

protected:
    LocalPlayerController(std::weak_ptr<Level> data) : PlayerController(std::move(data)) {
    }

};
