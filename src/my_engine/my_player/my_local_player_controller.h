#pragma once

#include "my_player/my_player_controller.h"

#include "my_framework/my_module.h"

class LocalPlayerController : public PlayerController {

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

};
